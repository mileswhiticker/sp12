#include "Structure.hpp"
#include "MapCell.hpp"
#include "Turf.hpp"

#include "CollisionDefines.h"
#include "Events.hpp"

#include <OGRE\OgreSceneNode.h>
#include <OGRE\OgreVector3.h>
#include <OGRE\OgreEntity.h>

/*const std::vector<std::string> Structure::QuadrantNames;
const std::vector<std::string> Structure::LevelNames;*/

Structure::Structure(Turf* a_pLocTurf, int a_Dir)
:	Atom(a_pLocTurf->m_pAtomRootSceneNode->getPosition(), a_Dir)
,	m_MyStructureType(UNKNOWNTYPE)
,	m_IsBuildPoint(false)
,	m_IsBlocking(true)
,	m_FaceQuadrant(UNKNOWNQUADRANT)
,	m_StructureLevel(0)
//,	m_pMountedOnStructure(NULL)
//,	m_pMountedGirder(NULL)
{
	//m_pSourceMapCell = a_pLocTurf->GetMapCell();
	m_MyAtomType = Atom::STRUCTURE;
	m_UseRigidbodyPosition = false;
	m_UsesGravity = false;
	m_DefaultPhysicsGroup = COLLISION_STRUCTURE;
	m_DefaultPhysicsMask = COLLISION_OBJ|COLLISION_MOB|COLLISION_STRUCTURE;

	m_pMyContext = new Context(a_Dir, m_FaceQuadrant, this);
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

void Structure::Select(InputModule* a_pSourceInputModule)
{
	Atom::Select(a_pSourceInputModule);
	if(m_pAtomEntity)
	{
		if(m_IsBuildPoint)
			m_pAtomEntity->setMaterialName(m_VirtualSelectMaterialName);
		else
			m_pAtomEntity->setMaterialName(m_SelectMaterialName);
	}
}

void Structure::DeSelect(InputModule* a_pSourceInputModule)
{
	Atom::DeSelect(a_pSourceInputModule);
	if(m_pAtomEntity)
	{
		m_pAtomEntity->setMaterialName(m_MaterialName);
	}
	if(m_IsBuildPoint)
	{
		SetEntityVisible(false);
	}
}

int Structure::GetStructureLevel()
{
	return m_StructureLevel;
}

int Structure::GetStructureQuadrant()
{
	return m_FaceQuadrant;
}
