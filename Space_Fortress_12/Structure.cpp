#include "Structure.hpp"

Structure::Structure()
:	Atom()
,	m_IsBuildPoint(false)
,	m_MyStructureType(UNKNOWN)
{
	//
}

void Structure::Instantiate()
{
	Instantiate(false);
}

bool Structure::IsBuildPoint()
{
	return m_IsBuildPoint;
}
