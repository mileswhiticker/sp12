#include "MapSuite.hpp"

#include "Application.hpp"
#include <OGRE\OgreSceneManager.h>

#include "iVector3.h"
#include "Files.hpp"
#include "tinyxml2.h"

#include <cmath>
#include <iostream>

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

						//resize the tilemap to make sure the new cell fits
						if(max >= m_MapCellGrid.size())
						{
							m_MapCellGrid.insert( m_MapCellGrid.end(), max - m_MapCellGrid.size() + 2, std::vector< std::vector<MapCell*> >() );

							//fill out xvector
							for(unsigned int x = 0; x < m_MapCellGrid.size(); x++)
							{
								if(max >= m_MapCellGrid[x].size())
								{
									m_MapCellGrid[x].insert( m_MapCellGrid[x].end(), max - m_MapCellGrid[x].size() + 2, std::vector<MapCell*>() );

									//fill out yvector
									for(unsigned int y = 0; y < m_MapCellGrid[x].size(); y++)
									{
										if(max >= m_MapCellGrid[x][y].size())
										{
											m_MapCellGrid[x][y].insert( m_MapCellGrid[x][y].end(), max - m_MapCellGrid[x][y].size() + 2, NULL );
										}
									}
								}
							}
						}

						//create the cell and slot it into the tilemap
						//iVector3 coords = iVector3(i, j, k);
						const char* skeleton_type = pElement->Attribute("skeleton");
						Girder* pGirder = NULL;
						const char* filling_type = NULL;
						if(!m_MapCellGrid[i][j][k] && skeleton_type)
						{
							MapCell* pLocMapCell = CreateNewMapCell(i,j,k);

							pGirder = (Girder*)AtomManager::GetSingleton().CreateStructure(Structure::GIRDER, pLocMapCell);
							pLocMapCell->m_pMyCellTurf = pGirder;
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

				//now that all the turfs have been created, loop over them and create adjacent girder buildpoints
				for(unsigned int x = 0; x < m_MapCellGrid.size() - 1; x++)
				{
					for(unsigned int y = 0; y < m_MapCellGrid[x].size() - 1; y++)
					{
						for(unsigned int z = 0; z < m_MapCellGrid[x][y].size() - 1; z++)
						{
							MapCell* pMapCell = m_MapCellGrid[x][y][z];
							if(pMapCell)
							{
								Turf* pTurf = m_MapCellGrid[x][y][z]->m_pMyCellTurf;
								if(pTurf)
								{
									if(pTurf->m_pMyStructure)
									{
										Structure* pStructure = pTurf->m_pMyStructure;
										if(!pStructure->IsBuildPoint())
										{
											CreateAdjacentGirderBuildpoints(x,y,z);
										}
									}
								}
							}
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

MapCell* MapSuite::CreateNewMapCell(unsigned int a_X, unsigned int a_Y, unsigned int a_Z)
{
	MapCell* pLocMapCell = new MapCell(Ogre::Vector3(Ogre::Real(a_X),Ogre::Real(a_Y),Ogre::Real(a_Z)));
	m_MapCellGrid[a_X][a_Y][a_Z] = pLocMapCell;
	if(a_X > 0)
	{
		pLocMapCell->m_pAdjWest = m_MapCellGrid[a_X - 1][a_Y][a_Z];
		if(m_MapCellGrid[a_X - 1][a_Y][a_Z])
			m_MapCellGrid[a_X - 1][a_Y][a_Z]->m_pAdjEast = pLocMapCell;
	}
	if(a_Y > 0)
	{
		pLocMapCell->m_pAdjDown = m_MapCellGrid[a_X][a_Y - 1][a_Z];
		if(m_MapCellGrid[a_X][a_Y - 1][a_Z])
			m_MapCellGrid[a_X][a_Y - 1][a_Z]->m_pAdjUp = pLocMapCell;
	}
	if(a_Z > 0)
	{
		pLocMapCell->m_pAdjSouth = m_MapCellGrid[a_X][a_Y][a_Z - 1];
		if(m_MapCellGrid[a_X][a_Y][a_Z - 1])
			m_MapCellGrid[a_X][a_Y][a_Z - 1]->m_pAdjNorth = pLocMapCell;
	}
	if(a_X < m_MapCellGrid.size() - 1)
	{
		pLocMapCell->m_pAdjEast = m_MapCellGrid[a_X + 1][a_Y][a_Z];
		if(m_MapCellGrid[a_X + 1][a_Y][a_Z])
			m_MapCellGrid[a_X + 1][a_Y][a_Z]->m_pAdjWest = pLocMapCell;
	}
	if(a_Y < m_MapCellGrid[a_X].size() - 1)
	{
		pLocMapCell->m_pAdjUp = m_MapCellGrid[a_X][a_Y + 1][a_Z];
		if(m_MapCellGrid[a_X][a_Y + 1][a_Z])
			m_MapCellGrid[a_X][a_Y + 1][a_Z]->m_pAdjDown = pLocMapCell;
	}
	if(a_Z < m_MapCellGrid[a_X][a_Y].size() - 1)
	{
		pLocMapCell->m_pAdjNorth = m_MapCellGrid[a_X][a_Y][a_Z + 1];
		if(m_MapCellGrid[a_X][a_Y][a_Z + 1])
			m_MapCellGrid[a_X][a_Y][a_Z + 1]->m_pAdjSouth = pLocMapCell;
	}

	return pLocMapCell;
}

int MapSuite::CreateAdjacentGirderBuildpoints(unsigned int a_X, unsigned int a_Y, unsigned int a_Z)
{
	int numCreated = 0;
	//limit the world size so that players can't crash the server by endlessly expanding
	//eventually, there should be some kind of instancing/IC mechanic to handle this, instead of an arbitrary invisible force field
	if(a_X + 1 > MAX_WORLD_RANGE || a_Y + 1 > MAX_WORLD_RANGE || a_Z + 1 > MAX_WORLD_RANGE)
		return numCreated;

	//expand the map to accomodate the new tiles
	if(a_X + 1 >= m_MapCellGrid.size())
	{
		for(unsigned int index=m_MapCellGrid.size(); index<=a_X+1; index++)
			m_MapCellGrid.push_back(std::vector< std::vector<MapCell*> >());
	}
	if(a_Y + 1 >= m_MapCellGrid[a_X].size())
	{
		for(unsigned int index=a_Y;index<=MAX_WORLD_RANGE;index++)
			m_MapCellGrid[a_X].push_back(std::vector<MapCell*>());
	}
	if(a_Z + 1 >= m_MapCellGrid.size())
	{
		for(unsigned int index=a_Z;index<=MAX_WORLD_RANGE;index++)
			m_MapCellGrid[a_X][a_Y].push_back(NULL);
	}
	//assume the passed co-ordinates are already valid for building around
	//if(m_MapCellGrid[a_X][a_Y][a_Z] && m_MapCellGrid[a_X][a_Y][a_Z]->GetAtomType() == Atom::STRUCTURE && !((Structure*)m_MapCellGrid[a_X][a_Y][a_Z])->IsBuildPoint())
	//surround this cell with highlighters

	//if there's nothing in the center cell, put a build highlighter here too
	if(!m_MapCellGrid[a_X + 0][a_Y + 0][a_Z + 0])
	{
		CreateNewMapCell(a_X + 0, a_Y + 0, a_Z + 0);
		//m_MapCellGrid[a_X + 1][a_Y + 0][a_Z + 0]->ResetEmptyOverlays();
	}
	if(!m_MapCellGrid[a_X + 0][a_Y + 0][a_Z + 0]->m_pMyCellTurf)
	{
		m_MapCellGrid[a_X + 1][a_Y + 0][a_Z + 0]->m_pMyCellTurf = (Girder*)AtomManager::GetSingleton().CreateStructureBuildpoint(Structure::GIRDER, m_MapCellGrid[a_X + 1][a_Y + 0][a_Z + 0]);
		//Ogre::Vector3(Ogre::Real(a_X + 1), Ogre::Real(a_Y + 0), Ogre::Real(a_Z + 0))
	}
	MapCell* pCenterMapCell = m_MapCellGrid[a_X][a_Y][a_Z];

	/*for(unsigned int x=a_X-1; x<=a_X+1; x++)
	{
		for(unsigned int y=a_X-1; y<=a_X+1; y++)
		{
			for(unsigned int z=a_X-1; z<=a_X+1; z++)
			{
				if(!m_MapCellGrid[x][y][z])
				{
					CreateNewMapCell(x, y, z);
				}
				if(!m_MapCellGrid[x][y][z]->m_pMyCellTurf)
				{
					m_MapCellGrid[x][y][z]->m_pMyCellTurf = (Girder*)AtomManager::GetSingleton().CreateStructureBuildpoint(Structure::GIRDER, m_MapCellGrid[x][y][z]);
				}
			}
		}
	}*/

	//---- do the adjacent cells
	if(!m_MapCellGrid[a_X + 1][a_Y + 0][a_Z + 0])
	{
		CreateNewMapCell(a_X + 1, a_Y + 0, a_Z + 0);
	}
	if(!m_MapCellGrid[a_X + 1][a_Y + 0][a_Z + 0]->m_pMyCellTurf)
	{
		m_MapCellGrid[a_X + 1][a_Y + 0][a_Z + 0]->m_pMyCellTurf = (Girder*)AtomManager::GetSingleton().CreateStructureBuildpoint(Structure::GIRDER, m_MapCellGrid[a_X + 1][a_Y + 0][a_Z + 0]);
	}
	//---- 
	
	//---- 
	if(!m_MapCellGrid[a_X - 1][a_Y + 0][a_Z + 0])
	{
		CreateNewMapCell(a_X - 1, a_Y + 0, a_Z + 0);
	}
	if(!m_MapCellGrid[a_X - 1][a_Y + 0][a_Z + 0]->m_pMyCellTurf)
	{
		m_MapCellGrid[a_X - 1][a_Y + 0][a_Z + 0]->m_pMyCellTurf = (Girder*)AtomManager::GetSingleton().CreateStructureBuildpoint(Structure::GIRDER, m_MapCellGrid[a_X - 1][a_Y + 0][a_Z + 0]);
	}
	//---- 
	
	//---- 
	if(!m_MapCellGrid[a_X - 1][a_Y + 0][a_Z + 0])
	{
	}
	if(!m_MapCellGrid[a_X - 1][a_Y + 0][a_Z + 0]->m_pMyCellTurf)
	{
		m_MapCellGrid[a_X - 1][a_Y + 0][a_Z + 0]->m_pMyCellTurf = (Girder*)AtomManager::GetSingleton().CreateStructureBuildpoint(Structure::GIRDER, m_MapCellGrid[a_X - 1][a_Y + 0][a_Z + 0]);
	}
	//---- 
	
	//---- 
	if(!m_MapCellGrid[a_X + 0][a_Y + 1][a_Z + 0])
	{
		CreateNewMapCell(a_X + 0, a_Y + 1, a_Z + 0);
	}
	if(!m_MapCellGrid[a_X + 0][a_Y + 1][a_Z + 0]->m_pMyCellTurf)
	{
		m_MapCellGrid[a_X + 0][a_Y + 1][a_Z + 0]->m_pMyCellTurf = (Girder*)AtomManager::GetSingleton().CreateStructureBuildpoint(Structure::GIRDER, m_MapCellGrid[a_X + 0][a_Y + 1][a_Z + 0]);
	}
	//---- 
	
	//---- 
	if(!m_MapCellGrid[a_X + 0][a_Y - 1][a_Z + 0])
	{
		CreateNewMapCell(a_X + 0, a_Y - 1, a_Z + 0);
	}
	if(!m_MapCellGrid[a_X + 0][a_Y - 1][a_Z + 0]->m_pMyCellTurf)
	{
		m_MapCellGrid[a_X + 0][a_Y - 1][a_Z + 0]->m_pMyCellTurf = (Girder*)AtomManager::GetSingleton().CreateStructureBuildpoint(Structure::GIRDER, m_MapCellGrid[a_X + 0][a_Y - 1][a_Z + 0]);
	}
	//---- 
	
	//---- 
	if(!m_MapCellGrid[a_X + 0][a_Y + 0][a_Z + 1])
	{
		CreateNewMapCell(a_X + 0, a_Y + 0, a_Z + 1);
	}
	if(!m_MapCellGrid[a_X + 0][a_Y + 0][a_Z + 1]->m_pMyCellTurf)
	{
		m_MapCellGrid[a_X + 0][a_Y + 0][a_Z + 1]->m_pMyCellTurf = (Girder*)AtomManager::GetSingleton().CreateStructureBuildpoint(Structure::GIRDER, m_MapCellGrid[a_X + 0][a_Y + 0][a_Z + 1]);
	}
	//---- 
	
	//---- 
	if(!m_MapCellGrid[a_X + 0][a_Y + 0][a_Z - 1])
	{
		CreateNewMapCell(a_X + 0, a_Y + 0, a_Z - 1);
	}
	if(!m_MapCellGrid[a_X + 0][a_Y + 0][a_Z - 1]->m_pMyCellTurf)
	{
		m_MapCellGrid[a_X + 0][a_Y + 0][a_Z - 1]->m_pMyCellTurf = (Girder*)AtomManager::GetSingleton().CreateStructureBuildpoint(Structure::GIRDER, m_MapCellGrid[a_X + 0][a_Y + 0][a_Z - 1]);
	}
	//---- 

	return numCreated;
}
