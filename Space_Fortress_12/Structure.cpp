#include "Structure.hpp"
#include "MapCell.hpp"

#include <OGRE\OgreVector3.h>

Structure::Structure(MapCell* a_pMapCell, int a_Dir)
:	Atom(a_pMapCell->m_Position, a_Dir)
,	m_IsBuildPoint(false)
,	m_MyStructureType(UNKNOWN)
//,	m_pMountedOnStructure(NULL)
//,	m_pMountedGirder(NULL)
{
	m_pSourceMapCell = a_pMapCell;
	m_MyAtomType = Atom::STRUCTURE;
	m_UseRigidbodyPosition = false;
	m_UsesGravity = false;
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
