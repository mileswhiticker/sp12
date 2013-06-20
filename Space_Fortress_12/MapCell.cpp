#include "MapCell.hpp"
#include "GravitySource.hpp"
#include "Atom.hpp"

MapCell::MapCell(Ogre::Vector3 a_Position)
:	m_pMyCellTurf(NULL)
//,	m_CombinedGravity(Ogre::Vector3::ZERO)
/*,	m_pAdjNorth(0)
,	m_pAdjSouth(0)
,	m_pAdjEast(0)
,	m_pAdjWest(0)
,	m_pAdjUp(0)
,	m_pAdjDown(0)*/
,	m_Position(a_Position)
{
	//
}

Ogre::Vector3 MapCell::GetGravity()
{
	Ogre::Vector3 out = Ogre::Vector3::ZERO;
	for(auto it = m_GravityAffectors.begin(); it != m_GravityAffectors.end(); ++it)
	{
		out += it->first->GetGravity() * it->second;
	}
	return out;
}

void MapCell::AddGravityAffector(GravitySource* a_pGravSource, float a_DistScalar)
{
	m_GravityAffectors.push_back( std::pair<GravitySource*,float>(a_pGravSource, a_DistScalar) );
	OnGravityChange();
}

void MapCell::RemoveGravityAffector(GravitySource* a_pGravSource)
{
	for(auto it = m_GravityAffectors.begin(); it != m_GravityAffectors.end(); ++it)
	{
		if(it->first == a_pGravSource)
		{
			m_GravityAffectors.erase(it);
			break;
		}
	}
	OnGravityChange();
}

void MapCell::AtomEnterCell(Atom* a_pEnteringAtom)
{
	m_AtomsInCell.push_back(a_pEnteringAtom);
	a_pEnteringAtom->OnGravityChange();
}

void MapCell::AtomLeaveCell(Atom* a_pLeavingAtom)
{
	for(auto it = m_AtomsInCell.begin(); it != m_AtomsInCell.end(); ++it)
	{
		if(*it == a_pLeavingAtom)
		{
			m_AtomsInCell.erase(it);
			break;
		}
	}
}

void MapCell::OnGravityChange()
{
	for(auto it = m_AtomsInCell.begin(); it != m_AtomsInCell.end(); ++it)
	{
		(*it)->OnGravityChange();
	}
}
