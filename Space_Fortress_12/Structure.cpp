#include "Structure.hpp"
#include "MapCell.hpp"
#include "Turf.hpp"
#include "Events.hpp"

#include <OGRE\OgreVector3.h>

Structure::Structure(Turf* a_pLocTurf, int a_Dir)
:	Atom(a_pLocTurf->GetSourceMapCell()->m_Position, a_Dir)
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

void Structure::Interact(Atom* a_pSourceAtom, InputModule* a_pSourceModule, int a_Intent, int a_Type)
{
	switch(a_Type)
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
	}
	Atom::Interact(a_pSourceAtom, a_pSourceModule, a_Intent, a_Type);
}

bool Structure::IsBuildPoint()
{
	return m_IsBuildPoint;
}

void Structure::Select(InputModule* a_pSelectingInputModule)
{
	Atom::Select(a_pSelectingInputModule);
}

void Structure::DeSelect(InputModule* a_pSelectingInputModule)
{
	Atom::DeSelect(a_pSelectingInputModule);
	if(m_IsBuildPoint)
	{
		SetEntityVisible(false);
	}
}
