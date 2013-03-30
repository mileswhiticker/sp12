#ifndef MAP_CELL_HPP
#define MAP_CELL_HPP

#include <OGRE/OgreVector3.h>
class Turf;

class MapCell
{
public:
	MapCell(Ogre::Vector3 a_Position);
	Ogre::Vector3 m_Position;
	//
	MapCell* m_pAdjNorth;
	MapCell* m_pAdjSouth;
	MapCell* m_pAdjEast;
	MapCell* m_pAdjWest;
	MapCell* m_pAdjUp;
	MapCell* m_pAdjDown;
	//
	Turf* m_pMyCellTurf;
};

#endif	MAP_CELL_HPP