#include "MapSuite.hpp"

#include "Application.hpp"
#include <OGRE\OgreSceneManager.h>

#include "iVector3.h"
#include "Files.hpp"
#include "tinyxml2.h"
#include <iostream>

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
				
				std::vector< std::vector< std::vector<Atom*> > > instanceGrid;	//3d matrix, each containing either a girder or a build highlight
				std::vector<PlayerSpawn*> loadedSpawns;	//list of all player spawns successfully loaded
				
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
							
							loadedSpawns.push_back(pNewSpawn);
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
						if(max >= instanceGrid.size())
						{
							instanceGrid.insert( instanceGrid.end(), max - instanceGrid.size() + 2, std::vector< std::vector<Atom*> >() );

							//fill out xvector
							for(unsigned int x = 0; x < instanceGrid.size(); x++)
							{
								if(max >= instanceGrid[x].size())
								{
									instanceGrid[x].insert( instanceGrid[x].end(), max - instanceGrid[x].size() + 2, std::vector<Atom*>() );

									//fill out yvector
									for(unsigned int y = 0; y < instanceGrid[x].size(); y++)
									{
										if(max >= instanceGrid[x][y].size())
										{
											instanceGrid[x][y].insert( instanceGrid[x][y].end(), max - instanceGrid[x][y].size() + 2, NULL );
										}
									}
								}
							}
						}

						//create the cell and slot it into the tilemap
						iVector3 coords = iVector3(i, j, k);
						const char* skeleton_type = pElement->Attribute("skeleton");
						Girder* pGirder = NULL;
						const char* filling_type = NULL;
						if(skeleton_type)
						{
							pGirder = (Girder*)AtomManager::GetSingleton().CreateAtom(Atom::GIRDER, coords.OgreVector3Copy());
							//std::string(pElement->Attribute("skeleton"))
							instanceGrid[i][j][k] = pGirder;
						
							//check if it wants to be filling in
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
										AtomManager::GetSingleton().CreateAtom(Atom::OBJECT, coords.OgreVector3Copy());
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

				//loop over loaded cells and create edge highlighters
				for(unsigned int x = 0; x < instanceGrid.size() - 1; x++)
				{
					for(unsigned int y = 0; y < instanceGrid[x].size() - 1; y++)
					{
						for(unsigned int z = 0; z < instanceGrid[x][y].size() - 1; z++)
						{
							if(instanceGrid[x][y][z] && instanceGrid[x][y][z]->GetAtomType() == Atom::GIRDER)
							{
								//surround this cell with highlighters
								if(!instanceGrid[x + 1][y + 0][z + 0])
								{
									instanceGrid[x + 1][y + 0][z + 0] = (Girder*)AtomManager::GetSingleton().CreateAtom(Atom::BUILDHIGHLIGHT, Ogre::Vector3(Ogre::Real(x + 1), Ogre::Real(y + 0), Ogre::Real(z + 0)));
									//instanceGrid[x + 1][y + 0][z + 0]->ResetEmptyOverlays();
								}
								if(!instanceGrid[x - 1][y + 0][z + 0])
								{
									instanceGrid[x - 1][y + 0][z + 0] = (Girder*)AtomManager::GetSingleton().CreateAtom(Atom::BUILDHIGHLIGHT, Ogre::Vector3(Ogre::Real(x - 1), Ogre::Real(y + 0), Ogre::Real(z + 0)));
									//new Girder(pStationSceneNode, Ogre::Vector3(Ogre::Real(x - 1), Ogre::Real(y + 0), Ogre::Real(z + 0)), "highlight_cell");
									//instanceGrid[x - 1][y + 0][z + 0]->ResetEmptyOverlays();
								}
								if(!instanceGrid[x + 0][y + 1][z + 0])
								{
									instanceGrid[x + 0][y + 1][z + 0] = (Girder*)AtomManager::GetSingleton().CreateAtom(Atom::BUILDHIGHLIGHT, Ogre::Vector3(Ogre::Real(x + 0), Ogre::Real(y + 1), Ogre::Real(z + 0)));
									//new Girder(pStationSceneNode, Ogre::Vector3(Ogre::Real(x + 0), Ogre::Real(y + 1), Ogre::Real(z + 0)), "highlight_cell");
									//instanceGrid[x + 0][y + 1][z + 0]->ResetEmptyOverlays();
								}
								if(!instanceGrid[x + 0][y - 1][z + 0])
								{
									instanceGrid[x + 0][y - 1][z + 0] = (Girder*)AtomManager::GetSingleton().CreateAtom(Atom::BUILDHIGHLIGHT, Ogre::Vector3(Ogre::Real(x + 0), Ogre::Real(y - 1), Ogre::Real(z + 0)));
									//new Girder(pStationSceneNode, Ogre::Vector3(Ogre::Real(x + 0), Ogre::Real(y - 1), Ogre::Real(z + 0)), "highlight_cell");
									//instanceGrid[x + 0][y - 1][z + 0]->ResetEmptyOverlays();
								}
								if(!instanceGrid[x + 0][y + 0][z + 1])
								{
									instanceGrid[x + 0][y + 0][z + 1] = (Girder*)AtomManager::GetSingleton().CreateAtom(Atom::BUILDHIGHLIGHT, Ogre::Vector3(Ogre::Real(x + 0), Ogre::Real(y + 0), Ogre::Real(z + 1)));
									//new Girder(pStationSceneNode, Ogre::Vector3(Ogre::Real(x + 0), Ogre::Real(y + 0), Ogre::Real(z + 1)), "highlight_cell");
									//instanceGrid[x + 0][y + 0][z + 1]->ResetEmptyOverlays();
								}
								if(!instanceGrid[x + 0][y + 0][z - 1])
								{
									instanceGrid[x + 0][y + 0][z - 1] = (Girder*)AtomManager::GetSingleton().CreateAtom(Atom::BUILDHIGHLIGHT, Ogre::Vector3(Ogre::Real(x + 0), Ogre::Real(y + 0), Ogre::Real(z - 1)));
									//new Girder(pStationSceneNode, Ogre::Vector3(Ogre::Real(x + 0), Ogre::Real(y + 0), Ogre::Real(z - 1)), "highlight_cell");
									//instanceGrid[x + 0][y + 0][z - 1]->ResetEmptyOverlays();
								}
							}
						}
					}
				}
				
				//display 2d slicemaps on console for debugging
				/*for(unsigned int y = 0; y < instanceGrid.size(); y++)
				{
					for(unsigned int z = 0; z < instanceGrid[y].size(); z++)
					{
						for(unsigned int x = 0; x < instanceGrid[y][z].size(); x++)
						{
							if(instanceGrid[x][y][z])
							{
								if(instanceGrid[x][y][z]->GetSkeletonType() == Girder::HIGHLIGHT)
									std::cout << "x";
								else if(instanceGrid[x][y][z]->GetSkeletonType() == Girder::GIRDER)
									std::cout << "O";
								else if(instanceGrid[x][y][z]->GetSkeletonType() == Girder::GIRDER_QUARTERS)
									std::cout << "H";
							}
							else
								std::cout << ".";
						}
						std::cout << std::endl;
					}
					std::cout << "/" << std::endl;
				}*/
				
				/*pStation->m_MapGirders = instanceGrid;
				pStation->m_PlayerSpawns = loadedSpawns;
				m_MapStations.push_back(pStation);*/
			}
		}

		return true;
	}

	//something went wrong
	std::cout << "ERROR code " << result << " while loading file \"" << full_file_path << "\"" << std::endl;
	return false;
}
