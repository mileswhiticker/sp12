#include "Structure.hpp"

Structure::Structure()
:	Atom()
,	m_IsBuildPoint(false)
,	m_MyStructureType(UNKNOWN)
,	m_pMountedOnStructure(NULL)
{
	//
}

void Structure::InstantiateAtom()
{
	InstantiateStructure(false);
}

bool Structure::IsBuildPoint()
{
	return m_IsBuildPoint;
}

void Structure::OwnedBuildPointCreated(Structure* a_pChild)
{
	for(auto it = m_UsedBuildPoints.begin(); it != m_UsedBuildPoints.end(); ++it)
	{
		if(*it == a_pChild)
		{
			m_UsedBuildPoints.push_back(*it);
			m_UnusedBuildPoints.erase(it);
		}
	}
}

Structure::StructureType Structure::GetStructureType()
{
	return m_MyStructureType;
}
