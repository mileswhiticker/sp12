#include "AtomManager.hpp"

#include "MapSuite.hpp"
#include "MapCell.hpp"

#include "Humanoid.hpp"
#include "Observer.hpp"
#include "Object.hpp"
//#include "Girder.hpp"
#include "Structure.hpp"
#include "OverlayPlating.hpp"
#include "UnderlayPlating.hpp"
#include "GravPlates.hpp"
#include "Turf.hpp"
#include "Box.hpp"
#include "Context.hpp"

#include "OgreHelper.hpp"
#include "Direction.h"

AtomManager::AtomManager()
:	cellsFlashing(false)
{
	//
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
	for(auto it = m_TurfsInWorldWantingUpdate.begin(); it != m_TurfsInWorldWantingUpdate.end(); ++it)
	{
		(*it).second->Update(a_DeltaT);
	}
}

Object* AtomManager::CreateObject(int a_ObjectType, Ogre::Vector3 a_Pos, int a_AdditionalFlags)
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

	return pOut;
}

Structure* AtomManager::CreateStructure(int a_StructureType, Turf* a_pLocTurf, int a_AdditionalFlags)
{
	Structure* pOut = NULL;
	switch(a_StructureType)
	{
	case(Structure::OVERLAYPLATING):
		{
			pOut = new OverlayPlating(a_pLocTurf, a_AdditionalFlags & ALLDIRS);
			
			//m_AtomsInWorld.insert(pOut);
			break;
		}
	case(Structure::UNDERLAYPLATING):
		{
			pOut = new UnderlayPlating(a_pLocTurf, a_AdditionalFlags & ALLDIRS);
			break;
		}
	case(Structure::GRAVPLATES):
		{
			pOut = new GravPlates(a_pLocTurf, a_AdditionalFlags & ALLDIRS);
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
		//if no turf is sent through, this is a "virtual" structure ie it doesn't really exist
		if(a_pLocTurf)
		{
			m_StructuresInWorld.insert(pOut);
			a_pLocTurf->AddStructure(pOut);
		}
		
		if(a_AdditionalFlags & INSTANTIATE_IMMEDIATELY)
		{
			pOut->InstantiateStructure(a_AdditionalFlags & BUILD_POINT ? true : false);
		}
	}

	return pOut;
}

void AtomManager::DeleteStructure(Structure* a_pStructureToDel)
{
	m_StructuresInWorld.erase(a_pStructureToDel);
	delete a_pStructureToDel;
}

Mob* AtomManager::CreateMob(int a_MobType, Ogre::Vector3 a_SpawnPos, int a_AdditionalFlags)
{
	Mob* pOut = NULL;
	switch(a_MobType)
	{
	case(Mob::OBSERVER):
		{
			pOut = new Observer(a_SpawnPos, a_AdditionalFlags & ALLDIRS);
			break;
		}
	case(Mob::HUMANOID):
		{
			pOut = new Humanoid(a_SpawnPos, a_AdditionalFlags & ALLDIRS);
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

	return pOut;
}

void AtomManager::DeleteMob(Mob* a_pMobToDel)
{
	m_MobsInWorld.erase(a_pMobToDel);
	delete a_pMobToDel;
}

//unused?
Context* AtomManager::CreateInteractContext(int a_Dir, int a_Quadrant, int a_Layer)
{
	return NULL;
}

void AtomManager::SetTurfWantingUpdate(Turf* a_pTurfToUpdate)
{
	if(a_pTurfToUpdate && !m_TurfsInWorldWantingUpdate.count(a_pTurfToUpdate->GetAtomUID()))
		m_TurfsInWorldWantingUpdate.insert( std::pair<int, Turf*>(a_pTurfToUpdate->GetAtomUID(), a_pTurfToUpdate) );
}

void AtomManager::StopTurfWantingUpdate(Turf* a_pTurfToStopUpdate)
{
	if(a_pTurfToStopUpdate)
	{
		auto it = m_TurfsInWorldWantingUpdate.find(a_pTurfToStopUpdate->GetAtomUID());
		if(it != m_TurfsInWorldWantingUpdate.end())
			m_TurfsInWorldWantingUpdate.erase(it);
	}
}
