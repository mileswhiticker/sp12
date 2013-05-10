#ifndef MAP_CELL_HPP
#define MAP_CELL_HPP

#include <OGRE/OgreVector3.h>
class Turf;
class GravitySource;

class MapCell
{
public:
	MapCell(Ogre::Vector3 a_Position);
	Ogre::Vector3 m_Position;
	Turf* m_pMyCellTurf;
	//
	void AddGravityAffector(GravitySource* a_pGravSource, float a_DistScalar);
	void RemoveGravityAffector(GravitySource* a_pGravSource);
	Ogre::Vector3 GetGravity();
	//
private:
	std::vector< std::pair<GravitySource*,float> > m_GravityAffectors;
	//
};

#endif	MAP_CELL_HPP