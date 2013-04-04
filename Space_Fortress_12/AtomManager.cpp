#include "AtomManager.hpp"

#include "Direction.h"
#include "MapSuite.hpp"
#include "Object.hpp"
#include "Girder.hpp"
#include "Structure.hpp"
#include "OverlayPlating.hpp"
#include "UnderlayPlating.hpp"
#include "MapCell.hpp"

#include "OgreHelper.hpp"

AtomManager::AtomManager()
:	cellsFlashing(false)
{
	//
}

void AtomManager::ToggleCellFlashing()
{
	/*cellsFlashing = !cellsFlashing;
	if(cellsFlashing)
	{
		for(auto it = m_GirdersInWorld.begin(); it != m_GirdersInWorld.end(); ++it)
		{
			(*it)->SetFlashingColour(Ogre::ColourValue(0,0,1,1));
		}
	}
	else
	{
		for(auto it = m_GirdersInWorld.begin(); it != m_GirdersInWorld.end(); ++it)
		{
			(*it)->StopFlashingColour();
		}
	}*/
}

void AtomManager::Update(float a_DeltaT)
{
	for(auto it = m_AtomsInWorld.begin(); it != m_AtomsInWorld.end(); ++it)
	{
		(*it)->Update(a_DeltaT);
	}
}

Atom* AtomManager::CreateAtom(int a_AtomType, Ogre::Vector3 a_Pos, bool a_InstantiateImmediately, Atom** a_ppAtomLocation)
{
	Atom* pOut = NULL;
	switch(a_AtomType)
	{
	case(Atom::OBJECT):
		{
			pOut = new Box(a_Pos);
			//m_ObjectsInWorld.push_back(pBox);
			//m_AtomsInWorld.insert(pBox);
			break;
		}
	default:
		{
			std::cout << "Unknown atom type: " << a_AtomType << std::endl;
			break;
		}
	}
	if(pOut && a_InstantiateImmediately)
	{
		pOut->InstantiateAtom();
		m_AtomsInWorld.insert(pOut);
	}

	if(a_ppAtomLocation)
	{
		*a_ppAtomLocation = pOut;
	}
	return pOut;
}

Structure* AtomManager::CreateStructure(int a_StructureType, MapCell* a_pLocMapCell, Structure** a_ppStructureLocation, int a_AdditionalFlags)
{
	Structure* pOut = NULL;
	switch(a_StructureType)
	{
	case(Structure::GIRDER):
		{
			pOut = new Girder(a_pLocMapCell);
			//m_GirdersInWorld.push_back((Girder*)pOut);
			//m_AtomsInWorld.insert(pOut);
			
			if( !(a_AdditionalFlags&BUILD_POINT) )
			{
				MapSuite::GetInstance().CreateAdjacentGirderBuildpoints(a_pLocMapCell);
			}
			break;
		}
	case(Structure::OVERLAYPLATING):
		{
			//todo
			pOut = new OverlayPlating(a_pLocMapCell->m_Position);
			
			//m_AtomsInWorld.insert(pOut);
			break;
		}
	case(Structure::UNDERLAYPLATING):
		{
			pOut = new UnderlayPlating(a_pLocMapCell->m_Position);
			break;
		}
	default:
		{
			std::cout << "Unknown structure type: " << a_StructureType << std::endl;
			break;
		}
	}

	//if creation was successful, deal with any extra flags
	if(pOut)
	{
		//only grab the direction flags, so we don't pass in unused extra bits
		pOut->ChangeDirection(a_AdditionalFlags & ALLDIRS);
		//std::cout << "direction set to " << (a_AdditionalFlags & ALLDIRS) << " " << a_AdditionalFlags << "/" << ALLDIRS << std::endl;

		m_AtomsInWorld.insert(pOut);
		
		if(a_AdditionalFlags & INSTANTIATE_IMMEDIATELY)
		{
			pOut->InstantiateStructure(a_AdditionalFlags & BUILD_POINT ? true : false);
		}
	}

	//update the passed in memaddress, regardless of whether creation was successful or not
	if(a_ppStructureLocation)
	{
		*a_ppStructureLocation = pOut;
	}
	return pOut;
}

void AtomManager::DeleteStructure(Structure* a_pStructureToDel)
{
	m_AtomsInWorld.erase(a_pStructureToDel);
	delete a_pStructureToDel;
}

/*bool AtomManager::CreateObject(std::string a_TypeTag, Ogre::Vector3 a_Position)
{
	if(!a_TypeTag.compare("box"))
	{
		Box* pBox = new Box(a_Position);
		m_ObjectsInWorld.push_back(pBox);
		m_AtomsInWorld.push_back(pBox);
		return true;
	}
	else
	{
		std::cout << "WARNING: Attempted to create object with Unknown type tag '" << a_TypeTag << "'" << std::endl;
	}
	return false;
}

Cell* AtomManager::CreateCell(Ogre::SceneNode* a_pSceneNode, Ogre::Vector3 a_Pos, std::string a_SkeletonType)
{
	m_CellsInWorld.push_back(new Cell(a_pSceneNode, a_Pos, a_SkeletonType));
	m_AtomsInWorld.push_back(m_CellsInWorld.back());

	return m_CellsInWorld.back();
}*/
