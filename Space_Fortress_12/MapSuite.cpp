#include "MapSuite.hpp"

#include "Application.hpp"
#include <OGRE\OgreSceneManager.h>

#include "Direction.h"
#include "iVector3.h"
#include "Files.hpp"
#include "tinyxml2.h"

#include <cmath>
#include <iostream>

#include "MapHelper.hpp"
#include "MapCell.hpp"
#include "MapDefines.h"
#include "Station.hpp"
#include "Girder.hpp"
#include "PlayerSpawn.hpp"

#include "AtomManager.hpp"

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

bool MapSuite::LoadMap(std::string a_FileName)
{
	return MapSuite::GetInstance().LoadMapFile(a_FileName);
}

//return value indicates success or failure
bool MapSuite::LoadMapFile(std::string a_FileName)
{
	std::string full_file_path = MAP_DIR + a_FileName + ".sf12m";
	tinyxml2::XMLDocument mapDoc;
	tinyxml2::XMLError result = mapDoc.LoadFile(full_file_path.c_str());
	if(!result)
	{
		//successful
		std::cout << "Successfully loaded file \"" << full_file_path << "\"" << std::endl;

		tinyxml2::XMLNode* pRootNode = mapDoc.FirstChild();
		for(tinyxml2::XMLNode* pTopLevelNode = pRootNode->FirstChild(); pTopLevelNode; pTopLevelNode = pTopLevelNode->NextSibling())
		{
			//new station or ship - a discrete, unconnectable bunch of cells
			tinyxml2::XMLElement* pTopLevelElement = pTopLevelNode->ToElement();
			if(!std::string("station").compare(pTopLevelNode->Value()) && pTopLevelElement)
			{
				//create the container object
				/*Station* pStation = new Station();
				pStation->m_Name = pTopLevelElement->Attribute("name");*/
				
				//Ogre::SceneNode* pStationSceneNode = Application::StaticGetSceneManager().getRootSceneNode()->createChildSceneNode(pStationName);
				for(tinyxml2::XMLNode* pGirderNode = pTopLevelNode->FirstChild(); pGirderNode; pGirderNode = pGirderNode->NextSibling())
				{
					if(!std::string("playerspawn").compare(pGirderNode->Value()))
					{
						tinyxml2::XMLElement* pElement = pGirderNode->ToElement();
						if(pElement)
						{
							int i = 0;
							if(pElement->QueryIntAttribute("x", &i))
								continue;
							int j = 0;
							if(pElement->QueryIntAttribute("y", &j))
								continue;
							int k = 0;
							if(pElement->QueryIntAttribute("z", &k))
								continue;
						
							PlayerSpawn* pNewSpawn = new PlayerSpawn();

							pNewSpawn->pos = Ogre::Vector3( Ogre::Real(i), Ogre::Real(j), Ogre::Real(k) );

							pElement->QueryIntAttribute("dir", &pNewSpawn->dir);

							const char* new_type = pElement->Attribute("type");
							if(new_type)
								pNewSpawn->type = std::string(new_type);
							
							m_LoadedSpawns.push_back(pNewSpawn);
						}
					}
					else if(!std::string("cell").compare(pGirderNode->Value()))
					{
						tinyxml2::XMLElement* pElement = pGirderNode->ToElement();
						assert(pElement);

						//if any of the attr loading fails, just skip this cell entirely
						int i = 0;
						if(pElement->QueryIntAttribute("x", &i))
							continue;
						int j = 0;
						if(pElement->QueryIntAttribute("y", &j))
							continue;
						int k = 0;
						if(pElement->QueryIntAttribute("z", &k))
							continue;
						
						//cube the entire tilemap for now so that all dimensions are equal
						unsigned int max = unsigned int(i) > unsigned int(j) ? unsigned int(i) : unsigned int(j);
						max = max > unsigned int(k) ? max : unsigned int(k);

						//create the cell and slot it into the tilemap
						//iVector3 coords = iVector3(i, j, k);
						const char* skeleton_type = pElement->Attribute("skeleton");
						Girder* pGirder = NULL;
						const char* filling_type = NULL;
						if(skeleton_type)
						{
							MapCell* pLocMapCell = CreateNewMapCell(i,j,k);

							pGirder = (Girder*)AtomManager::GetSingleton().CreateStructure(Structure::GIRDER, pLocMapCell, NULL, INSTANTIATE_IMMEDIATELY);
							//std::string(pElement->Attribute("skeleton"))
							//check if it wants to be filling in
							//todo: update this to new method
							filling_type = pElement->Attribute("filling");
							if(filling_type)
							{
								//add underlay plating in every direction
								pGirder->AddUnderlay(1, filling_type);
								pGirder->AddUnderlay(2, filling_type);
								pGirder->AddUnderlay(4, filling_type);
								pGirder->AddUnderlay(8, filling_type);
								pGirder->AddUnderlay(16, filling_type);
								pGirder->AddUnderlay(32, filling_type);
							}
						}

						//loop through cell nodes
						for(tinyxml2::XMLNode* pGirderAttributeNode = pGirderNode->FirstChild(); pGirderAttributeNode; pGirderAttributeNode = pGirderAttributeNode->NextSibling())
						{
							if(pGirder && !std::string("overlay").compare(pGirderAttributeNode->Value()))
							{
								//overlay plating
								tinyxml2::XMLElement* pElement = pGirderAttributeNode->ToElement();
								if(pElement)
								{
									int dir = 0;
									if(pElement->QueryIntAttribute("dir", &dir))
										continue;
									const char* plating = pElement->Attribute("plating");
									if(plating)
										pGirder->AddOverlay(dir, plating);
								}
							}
							else if(pGirder && !filling_type && !std::string("underlay").compare(pGirderAttributeNode->Value()))
							{
								//underlay plating
								tinyxml2::XMLElement* pElement = pGirderAttributeNode->ToElement();
								if(pElement)
								{
									int dir = 0;
									if(pElement->QueryIntAttribute("dir", &dir))
										continue;
									const char* plating = pElement->Attribute("plating");
									if(plating)
										pGirder->AddUnderlay(dir, plating);
								}
							}
							else if(!std::string("obj").compare(pGirderAttributeNode->Value()))
							{
								//free floating object
								tinyxml2::XMLElement* pElement = pGirderAttributeNode->ToElement();
								if(pElement)
								{
									std::string objTag = pElement->Attribute("tag");
									if(!objTag.compare("box"))
									{
										AtomManager::GetSingleton().CreateAtom(Atom::OBJECT, Ogre::Vector3(Ogre::Real(i),Ogre::Real(j),Ogre::Real(k)));
									}
								}
							}
						}

						if(pGirder)
						{
							pGirder->ResetEmptyOverlays();
						}
					}
				}

				//display 2d slicemaps on console for debugging
				/*for(unsigned int y = 0; y < m_MapCellGrid.size(); y++)
				{
					for(unsigned int z = 0; z < m_MapCellGrid[y].size(); z++)
					{
						for(unsigned int x = 0; x < m_MapCellGrid[y][z].size(); x++)
						{
							if(m_MapCellGrid[x][y][z])
							{
								if(m_MapCellGrid[x][y][z]->GetSkeletonType() == Girder::HIGHLIGHT)
									std::cout << "x";
								else if(m_MapCellGrid[x][y][z]->GetSkeletonType() == Girder::GIRDER)
									std::cout << "O";
								else if(m_MapCellGrid[x][y][z]->GetSkeletonType() == Girder::GIRDER_QUARTERS)
									std::cout << "H";
							}
							else
								std::cout << ".";
						}
						std::cout << std::endl;
					}
					std::cout << "/" << std::endl;
				}*/
				
				/*pStation->m_MapGirders = m_MapCellGrid;
				pStation->m_PlayerSpawns = m_LoadedSpawns;
				m_MapStations.push_back(pStation);*/
			}
		}

		return true;
	}

	//something went wrong
	std::cout << "ERROR code " << result << " while loading file \"" << full_file_path << "\"" << std::endl;
	return false;
}

MapCell* MapSuite::CreateNewMapCell(int a_X, int a_Y, int a_Z)
{
	return CreateNewMapCell(Ogre::Vector3(Ogre::Real(a_X),Ogre::Real(a_Y),Ogre::Real(a_Z)));
}

MapCell* MapSuite::CreateNewMapCell(Ogre::Vector3 a_Coords)
{
	MapCell* pNewMapCell = new MapCell(a_Coords);
	m_MapCellGrid.emplace(std::make_pair(GetCoordsString(a_Coords), pNewMapCell));

	return pNewMapCell;
}

int MapSuite::CreateAdjacentGirderBuildpoints(MapCell* a_pLocMapCell)
{
	int numCreated = 0;
	if(a_pLocMapCell)
	{
		//limit the world size so that players can't crash the server by endlessly expanding
		//eventually, there should be some kind of instancing/IC mechanic to handle this, instead of an arbitrary invisible force field
		//(limits disabled for now)
		/*if(a_X + 1 > MAX_WORLD_RANGE || a_Y + 1 > MAX_WORLD_RANGE || a_Z + 1 > MAX_WORLD_RANGE)
			return numCreated;*/

		//if there's nothing in the center cell, put a build highlighter here
		if(!a_pLocMapCell->m_pMyCellTurf)
		{
			AtomManager::GetSingleton().CreateStructure(Structure::GIRDER, a_pLocMapCell, NULL, INSTANTIATE_IMMEDIATELY|BUILD_POINT);
		}

		//loop through cardinal directions and add girder build points in them
		for(int curDir = 1; curDir < 32; curDir *= 2)
		{
			Ogre::Vector3 targetCoords = GetCoordsInDir(a_pLocMapCell->m_Position, curDir);
			MapCell* pCurrentCell = GetCellAtCoordsOrCreate(targetCoords);

			//only add a buildpoint if the cell doesn't have something there already
			if(!pCurrentCell->m_pMyCellTurf)
			{
				AtomManager::GetSingleton().CreateStructure(Structure::GIRDER, pCurrentCell, NULL, INSTANTIATE_IMMEDIATELY|BUILD_POINT);
			}
		}
	}

	return numCreated;
}

MapCell* MapSuite::GetCellInDirOrNull(MapCell* a_pSourceMapCell, int a_Direction)
{
	MapCell* pOut = NULL;
	if(a_pSourceMapCell)
	{
		Ogre::Vector3 newCoords = GetCoordsInDir(a_pSourceMapCell->m_Position, a_Direction);

		//todo: catch out of range exception
		pOut = m_MapCellGrid.at(GetCoordsString(newCoords));
	}
	return pOut;
}

MapCell* MapSuite::GetCellAtCoordsOrCreate(int a_X, int a_Y, int a_Z)
{
	MapCell* pOut = NULL;
	try
	{
		pOut = m_MapCellGrid.at(GetCoordsString(a_X, a_Y, a_Z));
	}
	catch (const std::out_of_range& oor)
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
