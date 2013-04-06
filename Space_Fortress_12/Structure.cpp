#include "Structure.hpp"

#include <OGRE\OgreVector3.h>

Structure::Structure(Ogre::Vector3 a_Pos, int a_Dir)
:	Atom(a_Pos, a_Dir)
,	m_IsBuildPoint(false)
,	m_MyStructureType(UNKNOWN)
,	m_pMountedOnStructure(NULL)
,	m_pMountedGirder(NULL)
{
	m_MyAtomType = Atom::STRUCTURE;
	m_UseRigidbodyPosition = false;
}

void Structure::InstantiateAtom()
{
	InstantiateStructure(false);
}

bool Structure::IsBuildPoint()
{
	return m_IsBuildPoint;
}

Structure::StructureType Structure::GetStructureType()
{
	return m_MyStructureType;
}

bool Structure::MountOnGirder(Girder* a_pMountTarget)
{
	if(!m_pMountedGirder)
	{
		m_pMountedGirder = a_pMountTarget;
		return true;
	}
	return false;
}

bool Structure::UnmountFromGirder()
{
	if(m_pMountedGirder)
	{
		m_pMountedGirder = NULL;
		return true;
	}
	return false;
}
