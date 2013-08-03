#include "MapSuite.hpp"

#include "Turf.hpp"
#include "MapHelper.hpp"
#include "AtomManager.hpp"
#include "Structure.hpp"
#include "RandHelper.h"

#include <OGRE\OgreSceneNode.h>

MapSuite::MapSuite()
{
	//
}

MapSuite& MapSuite::GetInstance()
{
	static MapSuite instance;
	return instance;
}

Station* MapSuite::GetStartingStation()
{
	if(MapSuite::GetInstance().m_MapStations.size() > 0)
	{
		return MapSuite::GetInstance().m_MapStations.front();
	}
	return NULL;
}

Turf* MapSuite::CreateTurf(int a_X, int a_Y, int a_Z, int a_TurfType)
{
	return CreateTurf(Ogre::Vector3(Ogre::Real(a_X),Ogre::Real(a_Y),Ogre::Real(a_Z)), a_TurfType);
}

Turf* MapSuite::CreateTurf(Ogre::Vector3 a_Coords, int a_TurfType)
{
	Turf* pNewTurf = AtomManager::GetSingleton().CreateTurf(a_TurfType, a_Coords, INSTANTIATE_IMMEDIATELY);
	//second param is false if it already exists

	std::pair<std::unordered_map<std::string, Turf*>::iterator, bool> result = m_TurfGrid.emplace(std::make_pair(GetCoordsString(a_Coords), pNewTurf));
	if(!result.second)
	{
		delete pNewTurf;
		pNewTurf = result.first->second;
	}

	return pNewTurf;
}

int MapSuite::CreateAdjacentGirderBuildpoints(Turf* a_pCenterTurf)
{
	int numCreated = 0;
	if(a_pCenterTurf)
	{
		//limit the world size so that players can't crash the server by endlessly expanding
		//eventually, there should be some kind of instancing/IC mechanic to handle this, instead of an arbitrary invisible force field
		//(limits disabled for now)
		/*if(a_X + 1 > MAX_WORLD_RANGE || a_Y + 1 > MAX_WORLD_RANGE || a_Z + 1 > MAX_WORLD_RANGE)
			return numCreated;*/

		//loop through cardinal directions and add girder build points in them
		for(int curDir = 1; curDir <= 32; curDir *= 2)
		{
			Ogre::Vector3 targetCoords = GetCoordsInDir(a_pCenterTurf->m_pAtomRootSceneNode->getPosition(), curDir);
			Turf* pCurrentTurf = GetTurfAtCoordsOrCreate(targetCoords);
			
			//only add a buildpoint if the cell doesn't have something there already
			if(pCurrentTurf)
			{
				pCurrentTurf->SetBuildable();
				//AtomManager::GetSingleton().CreateTurf(Turf::GIRDER, pCurrentTurf, INSTANTIATE_IMMEDIATELY|BUILD_POINT);
				numCreated++;
			}
			else
			{
				std::cout << "WARNING: Unable to create turf at " << targetCoords << std::endl;
			}
		}
	}
	
	//std::cout << numCreated << " buildpoints created" << std::endl;
	return numCreated;
}

int MapSuite::ClearDependantAdjacentGirderBuildpoints(Turf* a_pCenterTurf)
{
	int numCleared = 0;
	if(a_pCenterTurf)
	{
		//loop through cardinal directions and delete girder build points if they have no other connected turfs
		for(int curAdjDir = 1; curAdjDir <= 32; curAdjDir *= 2)
		{
			Ogre::Vector3 targetCoords = GetCoordsInDir(a_pCenterTurf->m_pAtomRootSceneNode->getPosition(), curAdjDir);
			Turf* pCurrentTurf = GetTurfAtCoordsOrNull(targetCoords);

			//check if there's a girder build point there
			if(pCurrentTurf \
				&& pCurrentTurf->GetTurfType() == Turf::GIRDER \
				&& pCurrentTurf->IsBuildPoint())
			{
				//see if there are any adjacent turfs other than the source cell
				int sourceDir = ReverseDir(curAdjDir);
				bool turfAdjacent = false;
				for(int curCheckDir = 1; curCheckDir <= 32; curCheckDir *= 2)
				{
					if(curCheckDir != sourceDir)
					{
						//see if there is a turf in that direction we can cling on to
						Turf* pCheckTurf = GetTurfInDirOrNull(pCheckTurf, curCheckDir);
						if(pCheckTurf && !pCheckTurf->IsBuildPoint())
						{
							turfAdjacent = true;
							break;
						}
					}
				}

				//if there's no adjacent turfs to "connect" the girder buildpoint to, clear this build point and it's associated mapcell
				//we've already checked above to make sure that the deletion is valid
				//the things we're deleting will make sure their own deletion is safe
				if(!turfAdjacent)
				{
					pCurrentTurf->SetBuildable(false);
					//AtomManager::GetSingleton().DeleteTurf(pCheckTurf);
					numCleared++;
					//leave the mapcell, gravity needs it
					//m_MapCellGrid.erase(GetCoordsString(pCurrentCell->m_Position));
					//delete pCurrentCell;
				}
			}
		}
	}

	//std::cout << numCleared << " buildpoints cleared" << std::endl;
	return numCleared;
}

Turf* MapSuite::GetTurfInDirOrNull(Turf* a_pSourceTurf, int a_Direction)
{
	Turf* pOut = NULL;
	if(a_pSourceTurf)
	{
		Ogre::Vector3 newCoords = GetCoordsInDir(a_pSourceTurf->m_pAtomRootSceneNode->getPosition(), a_Direction);
		
		try
		{
			pOut = m_TurfGrid.at(GetCoordsString(newCoords));
		}
		catch (const std::out_of_range& oor)
		{
			//nothing
		}
	}
	return pOut;
}

Turf* MapSuite::GetTurfInDirOrCreate(Turf* a_pSourceTurf, int a_Direction)
{
	Turf* pOut = NULL;
	if(a_pSourceTurf)
	{
		Ogre::Vector3 newCoords = GetCoordsInDir(a_pSourceTurf->m_pAtomRootSceneNode->getPosition(), a_Direction);
		pOut = GetTurfAtCoordsOrCreate(newCoords);
	}
	return pOut;
}

Turf* MapSuite::GetTurfAtCoordsOrCreate(int a_X, int a_Y, int a_Z)
{
	Turf* pOut = GetTurfAtCoordsOrNull(a_X, a_Y, a_Z);
	if(!pOut)
	{
		//if it doesn't exist, just create it
		pOut = CreateTurf(a_X, a_Y, a_Z);
	}
	return pOut;
}

Turf* MapSuite::GetTurfAtCoordsOrCreate(Ogre::Vector3 a_Coords)
{
	return GetTurfAtCoordsOrCreate(a_Coords.x, a_Coords.y, a_Coords.z);
}

Turf* MapSuite::GetTurfAtCoordsOrNull(int a_X, int a_Y, int a_Z)
{
	Turf* pOut = NULL;

	//offset due to rounding when casting from float to integer
	/*if(a_X < 0)
		a_X -= 1;
	if(a_Y < 0)
		a_Y -= 1;
	if(a_Z < 0)
		a_Z -= 1;*/

	try
	{
		pOut = m_TurfGrid.at(GetCoordsString(a_X, a_Y, a_Z));
	}
	catch (const std::out_of_range& oor)
	{
		//nothing
		pOut = NULL;
	}
	return pOut;
}

Turf* MapSuite::GetTurfAtCoordsOrNull(Ogre::Vector3 a_Coords)
{
	return GetTurfAtCoordsOrNull(a_Coords.x, a_Coords.y, a_Coords.z);
}

PlayerSpawn* MapSuite::GetRandomPlayerSpawn()
{
	if(m_LoadedSpawns.size())
	{
		return m_LoadedSpawns[iRand(m_LoadedSpawns.size())];
	}
	return NULL;
}
