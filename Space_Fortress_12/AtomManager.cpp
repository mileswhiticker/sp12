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
#include "Turf.hpp"

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

Turf* AtomManager::CreateTurf(int a_TurfType, MapCell* a_pLocMapCell, int a_AdditionalFlags)
{
	Turf* pOut = NULL;
	if(a_pLocMapCell)
	{
		switch(a_TurfType)
		{
		case(Turf::GIRDER):
			{
				if(a_AdditionalFlags & BUILD_POINT)
				{
					if(!a_pLocMapCell->m_pMyCellTurf)
					{
						pOut = new Girder(a_pLocMapCell);
					}
				}
				else
				{
					ClearMapCell(a_pLocMapCell);
					pOut = new Girder(a_pLocMapCell);
					MapSuite::GetInstance().CreateAdjacentGirderBuildpoints(a_pLocMapCell);
				}
				//m_GirdersInWorld.push_back((Girder*)pOut);
				//m_AtomsInWorld.insert(pOut);
				
				break;
			}
		default:
			{
				std::cout << "Unknown turf type: " << a_TurfType << std::endl;
				break;
			}
		}

		//if creation was successful, deal with any extra flags
		if(pOut)
		{
			m_TurfsInWorld.insert(pOut);
			if(a_AdditionalFlags & INSTANTIATE_IMMEDIATELY)
			{
				pOut->InstantiateTurf(a_AdditionalFlags & BUILD_POINT ? true : false);
			}
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
		m_StructuresInWorld.insert(pOut);
		
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

	return pOut;
}

void AtomManager::DeleteMob(Mob* a_pMobToDel)
{
	m_MobsInWorld.erase(a_pMobToDel);
	delete a_pMobToDel;
}

void AtomManager::DeleteTurf(Turf* a_pTurfToDel)
{
	if(a_pTurfToDel)
	{
		a_pTurfToDel->GetSourceMapCell()->m_pMyCellTurf = NULL;
		m_TurfsInWorld.erase(a_pTurfToDel);
		delete a_pTurfToDel;
	}
}

void AtomManager::ClearMapCell(MapCell* a_pMapCell)
{
	DeleteTurf(a_pMapCell->m_pMyCellTurf);
}
