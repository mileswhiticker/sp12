#ifndef MAP_CELL_HPP
#define MAP_CELL_HPP

#include <OGRE/OgreVector3.h>
class Turf;

class MapCell
{
public:
	MapCell(Ogre::Vector3 a_Position);
	Ogre::Vector3 m_Position;
	Turf* m_pMyCellTurf;
	//
	void AddGravityForce(Ogre::Vector3 a_AdditiveGravityForce);
	void RemoveGravityForce(Ogre::Vector3 a_SubtractiveGravityForce);
	Ogre::Vector3 GetGravity();
	//
private:
	Ogre::Vector3 m_CombinedGravity;
	//
};

#endif	MAP_CELL_HPP