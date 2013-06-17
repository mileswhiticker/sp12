#include "MapSuite.hpp"

#include "Application.hpp"
#include <OGRE\OgreSceneManager.h>

#include <cmath>
#include <iostream>

#include "OgreHelper.hpp"
#include "num2string.h"
#include "UID.hpp"
#include "MapDefines.h"
#include "Direction.h"
#include "iVector3.h"
#include "Files.hpp"

#include "Girder.hpp"
#include "PlayerSpawn.hpp"
#include "Object.hpp"

#include "AtomManager.hpp"
#include "tinyxml2.h"

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
				
				Ogre::SceneManager& sceneManager = GetSceneManager();
				//point light for testing
				/*Ogre::Light* pointLight = sceneManager.createLight("pointLight" + num2string(NewUID()));
				pointLight->setType(Ogre::Light::LT_POINT);
				pointLight->setPosition(10,5,0);
				pointLight->setDiffuseColour(1.0, 1.0, 1.0);
				//pointLight->setSpecularColour(0.0, 0.0, 1.0);
				pointLight->setAttenuation(25, 0, 1, 0);*/

				//light from a nearby star
				/*Ogre::Light* directionalLight = sceneManager.createLight("directionalLight" + num2string(NewUID()));
				directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
				directionalLight->setDirection(Ogre::Vector3::UNIT_X);
				directionalLight->setDiffuseColour(1,1,1);
				//directionalLight->setDiffuseColour(0.5,0.5,0.5);*/

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
						
						//create the cell and slot it into the tilemap
						//iVector3 coords = iVector3(i, j, k);
						Girder* pGirder = NULL;
						const char* filling_type = NULL;
						//std::string(pElement->Attribute("skeleton"))
						MapCell* pLocMapCell = CreateNewMapCell(i,j,k);
						pGirder = (Girder*)AtomManager::GetSingleton().CreateTurf(Turf::GIRDER, pLocMapCell, INSTANTIATE_IMMEDIATELY);

						//check if it wants to be filling in
						//todo: update this to new method
						/*filling_type = pElement->Attribute("filling");
						if(filling_type)
						{
							//add underlay plating in every direction
							pGirder->AddUnderlay(1, filling_type);
							pGirder->AddUnderlay(2, filling_type);
							pGirder->AddUnderlay(4, filling_type);
							pGirder->AddUnderlay(8, filling_type);
							pGirder->AddUnderlay(16, filling_type);
							pGirder->AddUnderlay(32, filling_type);
						}*/

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
									{
										//pGirder->AddUnderlay(dir, plating);
										//Structure* pUnusedBuildPoint = AtomManager::GetSingleton().CreateStructure(Structure::OVERLAYPLATING, pLocMapCell, NULL, dir|INSTANTIATE_IMMEDIATELY);
										pGirder->CreateBuildpointInDir(Structure::OVERLAYPLATING, dir);
									}
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
									{
										//pGirder->AddUnderlay(dir, plating);
										//Structure* pUnusedBuildPoint = AtomManager::GetSingleton().CreateStructure(Structure::UNDERLAYPLATING, pLocMapCell, NULL, dir|INSTANTIATE_IMMEDIATELY);
										pGirder->CreateBuildpointInDir(Structure::UNDERLAYPLATING, dir);
									}
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
										AtomManager::GetSingleton().CreateObject(Object::BOX, Ogre::Vector3(Ogre::Real(i),Ogre::Real(j),Ogre::Real(k)));
									}
								}
							}

							/*if(pGirder)
							{
								pGirder->ResetEmptyOverlays();
							}*/
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

