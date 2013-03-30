#include "MapCell.hpp"

MapCell::MapCell(Ogre::Vector3 a_Position)
:	m_pMyCellTurf(NULL)
,	m_pAdjNorth(0)
,	m_pAdjSouth(0)
,	m_pAdjEast(0)
,	m_pAdjWest(0)
,	m_pAdjUp(0)
,	m_pAdjDown(0)
,	m_Position(a_Position)
{
	//
}
