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

MapSuite& MapSuite::GetSingleton()
{
	static MapSuite instance;
	return instance;
}

Station* MapSuite::GetStartingStation()
{
	if(MapSuite::GetSingleton().m_MapStations.size() > 0)
	{
		return MapSuite::GetSingleton().m_MapStations.front();
	}
	return NULL;
}

/*
Turf* MapSuite::CreateTurf(int a_X, int a_Y, int a_Z, int a_TurfType, int a_AdditionalFlags)
{
	return CreateTurf(Ogre::Vector3(Ogre::Real(a_X),Ogre::Real(a_Y),Ogre::Real(a_Z)), a_TurfType);
}

Turf* MapSuite::CreateTurf(Ogre::Vector3 a_Coords, int a_TurfType, int a_AdditionalFlags)
{
	//create it
	Turf* pOut = new Turf(a_Coords);
	if(a_AdditionalFlags & INSTANTIATE_IMMEDIATELY)
	{
		pOut->Instantiate((Turf::TurfType)a_TurfType);
	}
	if(a_AdditionalFlags & BUILD_POINT)
	{
		//pOut->SetBuildpoint(true);
	}

	//second param is false if it already exists
	
	//m_TurfsInWorld.insert(pOut);
	std::pair<std::unordered_map<std::string, Turf*>::iterator, bool> result = m_TurfGrid.emplace(std::make_pair(GetCoordsString(a_Coords), pOut));

	//if a turf already exists, grab it instead
	if(!result.second)
	{
		delete pOut;		//ahaha not cleaning up this properly...
		pOut = result.first->second;
	}

	return pOut;
}
*/

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
			if(GetTurfAtCoordsOrNull(targetCoords))
				continue;

			Turf* pCurrentTurf = GetTurfAtCoordsOrCreate(targetCoords, Turf::BUILDTURF);
			
			//only add a buildpoint if the cell doesn't have something there already
			/*if(pCurrentTurf)
			{
				pCurrentTurf->SetBuildpoint();
				//AtomManager::GetSingleton().CreateTurf(Turf::GIRDER, pCurrentTurf, INSTANTIATE_IMMEDIATELY|BUILD_POINT);
				numCreated++;
			}
			else
			{
				std::cout << "WARNING: Unable to create turf at " << targetCoords << std::endl;
			}*/
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
			if(pCurrentTurf && pCurrentTurf->GetTurfType() == Turf::BUILDTURF)
			{
				//see if there are any adjacent turfs other than the source cell
				int sourceDir = ReverseDir(curAdjDir);
				bool turfAdjacent = false;
				for(int curCheckDir = 1; curCheckDir <= 32; curCheckDir *= 2)
				{
					//see if there is a turf in that direction we can cling on to
					Turf* pCheckTurf = GetTurfInDirOrNull(pCurrentTurf, curCheckDir);
					if(pCheckTurf && !pCheckTurf->GetTurfType() == Turf::GIRDER && pCheckTurf != a_pCenterTurf)		//girders can only be mounted on other girders... for now
					{
						turfAdjacent = true;
						break;
					}
				}

				//if there's no adjacent turfs to "connect" the girder buildpoint to, clear this build point and it's associated mapcell
				//we've already checked above to make sure that the deletion is valid
				//the things we're deleting will make sure their own deletion is safe
				if(!turfAdjacent)
				{
					DeleteTurf(pCurrentTurf);
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
			pOut = NULL;
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

Turf* MapSuite::GetTurfAtCoordsOrCreate(int a_X, int a_Y, int a_Z, int a_TurfType, int a_AdditionalFlags)
{
	return GetTurfAtCoordsOrCreate( Ogre::Vector3(a_X, a_Y, a_Z), a_TurfType, a_AdditionalFlags );
}

Turf* MapSuite::GetTurfAtCoordsOrCreate(Ogre::Vector3 a_Coords, int a_TurfType, int a_AdditionalFlags)
{
	Turf* pOut = GetTurfAtCoordsOrNull(a_Coords);
	if(!pOut)
	{
		//create it
		pOut = new Turf(a_Coords);

		if(a_AdditionalFlags & INSTANTIATE_IMMEDIATELY)
		{
			//do not override turf type if there's already a turf... not sure if we would even need to do that
			pOut->Instantiate((Turf::TurfType)a_TurfType);
		}

		std::pair<std::unordered_map<std::string, Turf*>::iterator, bool> result = m_TurfGrid.emplace(std::make_pair(GetCoordsString(a_Coords), pOut));
		
		if(a_AdditionalFlags & CREATE_BUILDTURFS)
		{
			if(a_TurfType > Turf::BUILDTURF && a_TurfType < Turf::TURF_MAXTYPE)
			{
				CreateAdjacentGirderBuildpoints(pOut);
			}
		}

		//this should be entirely redundant
		/*if(!result.second)
		{
			delete pOut;		//ahaha not cleaning up this properly...
			pOut = result.first->second;
		}*/
	}
	return pOut;
}

Turf* MapSuite::GetTurfAtCoordsOrNull(Ogre::Vector3 a_Coords)
{
	return GetTurfAtCoordsOrNull(a_Coords.x, a_Coords.y, a_Coords.z);
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

PlayerSpawn* MapSuite::GetRandomPlayerSpawn()
{
	if(m_LoadedSpawns.size())
	{
		return m_LoadedSpawns[iRand(m_LoadedSpawns.size())];
	}
	return NULL;
}


void MapSuite::DeleteTurf(Turf* a_pTurfToDel)
{
	if(a_pTurfToDel)
	{
		Ogre::Vector3 turfCoords = a_pTurfToDel->m_pAtomRootSceneNode->getPosition();
		try
		{
			m_TurfGrid.erase( GetCoordsString(turfCoords) );
		}
		catch (const std::out_of_range& oor)
		{
			//couldn't find it
			std::cout << "Warning, deleting turf not located in worldmap!" << std::endl;
		}

		if(a_pTurfToDel->GetTurfType() == Turf::GIRDER)
		{
			ClearDependantAdjacentGirderBuildpoints(a_pTurfToDel);
		}

		//m_TurfsInWorld.erase(a_pTurfToDel);

		//see if we need to create a new buildturf here
		bool turfAdjacent = false;
		for(int curCheckDir = 1; curCheckDir <= 32; curCheckDir *= 2)
		{
			//see if there is a turf in that direction we can cling on to
			Turf* pCheckTurf = GetTurfInDirOrNull(a_pTurfToDel, curCheckDir);
			if(pCheckTurf && pCheckTurf->GetTurfType() == Turf::GIRDER)		//girders can only be mounted on other girders... for now
			{
				turfAdjacent = true;
				break;
			}
		}

		delete a_pTurfToDel;

		if(turfAdjacent)
		{
			Turf* pCurrentTurf = GetTurfAtCoordsOrCreate(turfCoords, Turf::BUILDTURF);
		}
	}
}

/*
void MapSuite::ClearMapCell(MapCell* a_pMapCell)
{
	DeleteTurf(a_pMapCell->m_pMyCellTurf);
}
*/

void MapSuite::BuildTurf(Turf* a_pBuildTurf, int a_TurfType)
{
	if(a_pBuildTurf && a_pBuildTurf->GetTurfType() == Turf::BUILDTURF)
	{
		a_pBuildTurf->Instantiate((Turf::TurfType)a_TurfType);
		a_pBuildTurf->SetEntityVisible();
		CreateAdjacentGirderBuildpoints(a_pBuildTurf);
	}
}
