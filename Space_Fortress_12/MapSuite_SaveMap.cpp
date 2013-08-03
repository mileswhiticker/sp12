#include "MapSuite.hpp"
#include "AtomManager.hpp"

#include "Turf.hpp"
#include "MapCell.hpp"
#include <OGRE\OgreSceneNode.h>

#include "RandHelper.h"
#include "num2string.h"
#include "tinyxml2.h"
#include "Files.hpp"
#include <time.h>

bool MapSuite::SaveMapDefaultName()
{
	//warning, no safety checks if the file already exists
	return SaveMap("mapsave_" + num2string((unsigned int)time(0)));
}

bool MapSuite::SaveMap(std::string a_FileName)
{
	//check if the a_Filename already exists
	//todo (just overwrite it for now)
	/*
	std::string full_file_path = MAP_DIR + a_FileName + ".sf12m";
	tinyxml2::XMLDocument mapDoc;
	AtomManager& atomManager = AtomManager::GetSingleton();

	//write map data to XMLdoc
	tinyxml2::XMLElement* pRootNode = mapDoc.NewElement("sf12m");
	mapDoc.InsertEndChild(pRootNode);
	pRootNode->InsertEndChild(mapDoc.NewComment("Experimental map format for Space Fortress 12"));
	pRootNode->InsertEndChild(mapDoc.NewComment("Use discrete integers for coordinatess"));

	tinyxml2::XMLElement* pStationNode = mapDoc.NewElement("station");
	pRootNode->InsertEndChild(pStationNode);
	pStationNode->SetAttribute("name", "main_station");

	for(auto it = atomManager.m_TurfsInWorld.begin(); it != atomManager.m_TurfsInWorld.end(); ++it)
	{
		if((*it)->GetTurfType() == Turf::GIRDER && !(*it)->IsBuildPoint())
		{
			Girder* pGirder = (Girder*)*it;
			MapCell* pMapCell = pGirder->GetSourceMapCell();
			if(pMapCell)
			{
				//create the cell node
				tinyxml2::XMLElement* pGirderNode = mapDoc.NewElement("cell");
				pStationNode->InsertEndChild(pGirderNode);

				//add co-ordinate attributes
				pGirderNode->SetAttribute("x", pMapCell->m_Position.x);
				pGirderNode->SetAttribute("y", pMapCell->m_Position.y);
				pGirderNode->SetAttribute("z", pMapCell->m_Position.z);

				//loop over buildpoints and add them if they're instantiated
				//for
			}
			else
			{
				std::cout << "WARNING: Girder without a map cell at " << pGirder->m_pAtomRootSceneNode->getPosition();
			}
		}
	}

	FILE* pFile = fopen(full_file_path.c_str(), "w");
	tinyxml2::XMLPrinter printer(pFile);
	mapDoc.Print(&printer);
	fclose(pFile);
	*/
	return true;
}
