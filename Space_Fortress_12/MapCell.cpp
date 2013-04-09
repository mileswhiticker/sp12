#include "MapCell.hpp"

MapCell::MapCell(Ogre::Vector3 a_Position)
:	m_pMyCellTurf(NULL)
,	m_CombinedGravity(Ogre::Vector3::ZERO)
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

void MapCell::AddGravityForce(Ogre::Vector3 a_AdditiveGravityForce)
{
	m_CombinedGravity += a_AdditiveGravityForce;
}

void MapCell::RemoveGavityForce(Ogre::Vector3 a_SubtractiveGravityForce)
{
	m_CombinedGravity -= a_SubtractiveGravityForce;
}

Ogre::Vector3 MapCell::GetGravity()
{
	return m_CombinedGravity;
}
