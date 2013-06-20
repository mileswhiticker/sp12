#include "MapSuite.hpp"

#include "MapCell.hpp"
#include "MapHelper.hpp"
#include "AtomManager.hpp"
#include "Structure.hpp"
#include "Turf.hpp"
#include "RandHelper.h"

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

MapCell* MapSuite::CreateNewMapCell(int a_X, int a_Y, int a_Z)
{
	return CreateNewMapCell(Ogre::Vector3(Ogre::Real(a_X),Ogre::Real(a_Y),Ogre::Real(a_Z)));
}

MapCell* MapSuite::CreateNewMapCell(Ogre::Vector3 a_Coords)
{
	MapCell* pNewMapCell = new MapCell(a_Coords);
	//second param is false if it already exists

	std::pair<std::unordered_map<std::string, MapCell*>::iterator, bool> result = m_MapCellGrid.emplace(std::make_pair(GetCoordsString(a_Coords), pNewMapCell));
	if(!result.second)
	{
		delete pNewMapCell;
		pNewMapCell = result.first->second;
	}

	return pNewMapCell;
}

int MapSuite::CreateAdjacentGirderBuildpoints(MapCell* a_pCenterMapCell)
{
	int numCreated = 0;
	if(a_pCenterMapCell)
	{
		//limit the world size so that players can't crash the server by endlessly expanding
		//eventually, there should be some kind of instancing/IC mechanic to handle this, instead of an arbitrary invisible force field
		//(limits disabled for now)
		/*if(a_X + 1 > MAX_WORLD_RANGE || a_Y + 1 > MAX_WORLD_RANGE || a_Z + 1 > MAX_WORLD_RANGE)
			return numCreated;*/

		//loop through cardinal directions and add girder build points in them
		for(int curDir = 1; curDir <= 32; curDir *= 2)
		{
			Ogre::Vector3 targetCoords = GetCoordsInDir(a_pCenterMapCell->m_Position, curDir);
			MapCell* pCurrentCell = GetCellAtCoordsOrCreate(targetCoords);

			//only add a buildpoint if the cell doesn't have something there already
			if(!pCurrentCell->m_pMyCellTurf)
			{
				AtomManager::GetSingleton().CreateTurf(Turf::GIRDER, pCurrentCell, INSTANTIATE_IMMEDIATELY|BUILD_POINT);
				numCreated++;
			}
		}
	}
	
	//std::cout << numCreated << " buildpoints created" << std::endl;
	return numCreated;
}

int MapSuite::ClearDependantAdjacentGirderBuildpoints(MapCell* a_pLocMapCell)
{
	int numCleared = 0;
	if(a_pLocMapCell)
	{
		//loop through cardinal directions and delete girder build points if they have no other connected turfs
		for(int curAdjDir = 1; curAdjDir <= 32; curAdjDir *= 2)
		{
			Ogre::Vector3 targetCoords = GetCoordsInDir(a_pLocMapCell->m_Position, curAdjDir);
			MapCell* pCurrentCell = GetCellAtCoordsOrNull(targetCoords);

			//check if there's a girder build point there
			if(pCurrentCell \
				&& pCurrentCell->m_pMyCellTurf \
				&& pCurrentCell->m_pMyCellTurf->GetTurfType() == Turf::GIRDER \
				&& pCurrentCell->m_pMyCellTurf->IsBuildPoint())
			{
				//see if there are any adjacent turfs other than the source cell
				int sourceDir = ReverseDir(curAdjDir);
				bool turfAdjacent = false;
				for(int curCheckDir = 1; curCheckDir <= 32; curCheckDir *= 2)
				{
					if(curCheckDir != sourceDir)
					{
						//see if there is a turf in that direction we can cling on to
						MapCell* pCheckCellForTurf = GetCellInDirOrNull(pCurrentCell, curCheckDir);
						if(pCheckCellForTurf && pCheckCellForTurf->m_pMyCellTurf \
							&& !pCheckCellForTurf->m_pMyCellTurf->IsBuildPoint())
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
					AtomManager::GetSingleton().DeleteTurf(pCurrentCell->m_pMyCellTurf);
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

MapCell* MapSuite::GetCellInDirOrNull(MapCell* a_pSourceMapCell, int a_Direction)
{
	MapCell* pOut = NULL;
	if(a_pSourceMapCell)
	{
		Ogre::Vector3 newCoords = GetCoordsInDir(a_pSourceMapCell->m_Position, a_Direction);
		
		try
		{
			pOut = m_MapCellGrid.at(GetCoordsString(newCoords));
		}
		catch (const std::out_of_range& oor)
		{
			//nothing
		}
	}
	return pOut;
}

MapCell* MapSuite::GetCellInDirOrCreate(MapCell* a_pSourceMapCell, int a_Direction)
{
	MapCell* pOut = NULL;
	if(a_pSourceMapCell)
	{
		Ogre::Vector3 newCoords = GetCoordsInDir(a_pSourceMapCell->m_Position, a_Direction);
		pOut = GetCellAtCoordsOrCreate(newCoords);
	}
	return pOut;
}

MapCell* MapSuite::GetCellAtCoordsOrCreate(int a_X, int a_Y, int a_Z)
{
	MapCell* pOut = GetCellAtCoordsOrNull(a_X, a_Y, a_Z);
	if(!pOut)
	{
		//if it doesn't exist, just create it
		pOut = CreateNewMapCell(a_X, a_Y, a_Z);
	}
	return pOut;
}

MapCell* MapSuite::GetCellAtCoordsOrCreate(Ogre::Vector3 a_Coords)
{
	return GetCellAtCoordsOrCreate(a_Coords.x, a_Coords.y, a_Coords.z);
}

MapCell* MapSuite::GetCellAtCoordsOrNull(int a_X, int a_Y, int a_Z)
{
	MapCell* pOut = NULL;

	//offset due to rounding when casting from float to integer
	/*if(a_X < 0)
		a_X -= 1;
	if(a_Y < 0)
		a_Y -= 1;
	if(a_Z < 0)
		a_Z -= 1;*/

	try
	{
		pOut = m_MapCellGrid.at(GetCoordsString(a_X, a_Y, a_Z));
	}
	catch (const std::out_of_range& oor)
	{
		//nothing
		pOut = NULL;
	}
	return pOut;
}

MapCell* MapSuite::GetCellAtCoordsOrNull(Ogre::Vector3 a_Coords)
{
	return GetCellAtCoordsOrNull(a_Coords.x, a_Coords.y, a_Coords.z);
}

PlayerSpawn* MapSuite::GetRandomPlayerSpawn()
{
	if(m_LoadedSpawns.size())
	{
		return m_LoadedSpawns[iRand(m_LoadedSpawns.size())];
	}
	return NULL;
}
