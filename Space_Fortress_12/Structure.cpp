#include "Structure.hpp"

Structure::Structure()
:	Atom()
,	m_IsBuildPoint(false)
,	m_MyStructureType(UNKNOWN)
,	m_pMountedOnStructure(NULL)
,	m_pMountedGirder(NULL)
{
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
