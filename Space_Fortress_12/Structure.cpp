#include "Structure.hpp"
#include "MapCell.hpp"
#include "Turf.hpp"
#include "Events.hpp"

#include <OGRE\OgreSceneNode.h>
#include <OGRE\OgreVector3.h>
#include <OGRE\OgreEntity.h>

Structure::Structure(Turf* a_pLocTurf, int a_Dir)
:	Atom(a_pLocTurf->m_pAtomRootSceneNode->getPosition(), a_Dir)
,	m_MyStructureType(UNKNOWN)
,	m_IsBuildPoint(false)
//,	m_pMountedOnStructure(NULL)
//,	m_pMountedGirder(NULL)
{
	//m_pSourceMapCell = a_pLocTurf->GetMapCell();
	m_MyAtomType = Atom::STRUCTURE;
	m_UseRigidbodyPosition = false;
	m_UsesGravity = false;
}

void Structure::InstantiateAtom()
{
	InstantiateStructure(false);
}

Structure::StructureType Structure::GetStructureType()
{
	return m_MyStructureType;
}

//unused
bool Structure::MountOnGirder(Girder* a_pMountTarget)
{
	if(!m_pMountedGirder)
	{
		m_pMountedGirder = a_pMountTarget;
		return true;
	}
	return false;
}

//unused
bool Structure::UnmountFromGirder()
{
	if(m_pMountedGirder)
	{
		m_pMountedGirder = NULL;
		return true;
	}
	return false;
}

bool Structure::Interact(Mob* a_pSourceMob, Context* a_pSourceContext, int a_InteractType, Atom* a_pUsedAtom)
{
	/*switch(a_Type)
	{
	case(Event::TOGGLEBUILD):
		{
			if(m_IsBuildPoint)
			{
				CreateFromBuildPoint();
			}
			else
			{
				DestroyToBuildPoint();
			}
			return;
		}
	case(Event::BUILD):
		{
			CreateFromBuildPoint();
			return;
		}
	case(Event::UNBUILD):
		{
			DestroyToBuildPoint();
			return;
		}
	}*/
	return Atom::Interact(a_pSourceMob, a_pSourceContext, a_InteractType, a_pUsedAtom);
}

bool Structure::IsBuildPoint()
{
	return m_IsBuildPoint;
}

void Structure::Select(Component* a_pSourceComponent)
{
	Atom::Select(a_pSourceComponent);
	if(m_pAtomEntity)
	{
		if(m_IsBuildPoint)
			m_pAtomEntity->setMaterialName(m_VirtualSelectMaterialName);
		else
			m_pAtomEntity->setMaterialName(m_SelectMaterialName);
	}
}

void Structure::DeSelect(Component* a_pSourceComponent)
{
	Atom::DeSelect(a_pSourceComponent);
	if(m_pAtomEntity)
	{
		m_pAtomEntity->setMaterialName(m_MaterialName);
	}
	if(m_IsBuildPoint)
	{
		SetEntityVisible(false);
	}
}
