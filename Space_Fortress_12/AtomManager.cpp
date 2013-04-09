#include "AtomManager.hpp"

#include "MapSuite.hpp"
#include "MapCell.hpp"

#include "Human.hpp"
#include "Observer.hpp"
#include "Object.hpp"
#include "Girder.hpp"
#include "Structure.hpp"
#include "OverlayPlating.hpp"
#include "UnderlayPlating.hpp"
#include "GravPlates.hpp"

#include "OgreHelper.hpp"
#include "Direction.h"

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
	for(auto it = m_StructuresInWorld.begin(); it != m_StructuresInWorld.end(); ++it)
	{
		(*it)->Update(a_DeltaT);
	}
	for(auto it = m_MobsInWorld.begin(); it != m_MobsInWorld.end(); ++it)
	{
		(*it)->Update(a_DeltaT);
	}
	for(auto it = m_ObjectsInWorld.begin(); it != m_ObjectsInWorld.end(); ++it)
	{
		(*it)->Update(a_DeltaT);
	}
}

Object* AtomManager::CreateObject(int a_ObjectType, Ogre::Vector3 a_Pos, Atom** a_ppAtomLocation, int a_AdditionalFlags)
{
	Object* pOut = NULL;
	switch(a_ObjectType)
	{
	case(Object::BOX):
		{
			pOut = new Box(a_Pos);
			//m_ObjectsInWorld.push_back(pBox);
			//m_AtomsInWorld.insert(pBox);
			break;
		}
	default:
		{
			std::cout << "Unknown object type: " << a_ObjectType << std::endl;
			break;
		}
	}
	if(pOut)
	{
		m_ObjectsInWorld.insert(pOut);

		//deal with any additional flags
		//ignore dir for now, worry about it later
		if(a_AdditionalFlags & INSTANTIATE_IMMEDIATELY)
		{
			pOut->InstantiateAtom();
		}
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
			
			if( !(a_AdditionalFlags & BUILD_POINT) )
			{
				MapSuite::GetInstance().CreateAdjacentGirderBuildpoints(a_pLocMapCell);
			}
			break;
		}
	case(Structure::OVERLAYPLATING):
		{
			pOut = new OverlayPlating(a_pLocMapCell, a_AdditionalFlags & ALLDIRS);
			
			//m_AtomsInWorld.insert(pOut);
			break;
		}
	case(Structure::UNDERLAYPLATING):
		{
			pOut = new UnderlayPlating(a_pLocMapCell, a_AdditionalFlags & ALLDIRS);
			break;
		}
	case(Structure::GRAVPLATES):
		{
			pOut = new GravPlates(a_pLocMapCell, a_AdditionalFlags & ALLDIRS);
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
		m_StructuresInWorld.insert(pOut);
		
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
	m_StructuresInWorld.erase(a_pStructureToDel);
	delete a_pStructureToDel;
}

Mob* AtomManager::CreateMob(int a_MobType, Ogre::Vector3 a_SpawnPos, Mob** a_ppAtomLocation, int a_AdditionalFlags)
{
	Mob* pOut = NULL;
	switch(a_MobType)
	{
	case(Mob::OBSERVER):
		{
			pOut = new Observer(a_SpawnPos, a_AdditionalFlags & ALLDIRS);
			break;
		}
	case(Mob::HUMAN):
		{
			pOut = new Human(a_SpawnPos, a_AdditionalFlags & ALLDIRS);
			break;
		}
	default:
		{
			std::cout << "Unknown mob type: " << a_MobType << std::endl;
			break;
		}
	}

	//if creation was successful, deal with any extra flags
	if(pOut)
	{
		m_MobsInWorld.insert(pOut);
		if(a_AdditionalFlags & INSTANTIATE_IMMEDIATELY)
		{
			pOut->InstantiateAtom();
		}
	}

	//update the passed in memaddress, regardless of whether creation was successful or not
	if(a_ppAtomLocation)
	{
		*a_ppAtomLocation = pOut;
	}
	return pOut;
}

void AtomManager::DeleteMob(Mob* a_pMobToDel)
{
	m_MobsInWorld.erase(a_pMobToDel);
	delete a_pMobToDel;
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
