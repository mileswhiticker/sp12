#include "Turf.hpp"
#include "MapCell.hpp"
#include "Events.hpp"

#include <OGRE\OgreSceneNode.h>
#include "MapCell.hpp"

Turf::Turf(MapCell* a_pLocMapCell)
:	Atom(a_pLocMapCell->m_Position)
,	m_pTurfStructure(0)
,	m_MyTurfType(UNKNOWN)
,	m_IsBuildPoint(true)
{
	m_MyAtomType = Atom::TURF;
	m_pSourceMapCell = a_pLocMapCell;
	if(m_pSourceMapCell)
	{
		m_pAtomRootSceneNode->setPosition(m_pSourceMapCell->m_Position);
		m_pSourceMapCell->m_pMyCellTurf = this;
	}
}

void Turf::InstantiateAtom()
{
	InstantiateTurf(false);
}

Turf::TurfType Turf::GetTurfType()
{
	return m_MyTurfType;
}

void Turf::Interact(Atom* a_pSourceAtom, InputModule* a_pSourceModule, int a_Intent, int a_Type)
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
	//just pass it up the chain
	Atom::Interact(a_pSourceAtom, a_pSourceModule, a_Intent, a_Type);
}

void Turf::Select(InputModule* a_pSelectingInputModule)
{
	Atom::Select(a_pSelectingInputModule);
}

void Turf::DeSelect(InputModule* a_pSelectingInputModule)
{
	Atom::DeSelect(a_pSelectingInputModule);
	if(m_IsBuildPoint)
	{
		SetEntityVisible(false);
	}
}

bool Turf::IsBuildPoint()
{
	return m_IsBuildPoint;
}
