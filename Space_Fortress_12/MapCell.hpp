#ifndef MAP_CELL_HPP
#define MAP_CELL_HPP

#include <OGRE/OgreVector3.h>
class GravitySource;
class Atom;
class Turf;
class Mob;
class Object;
class Structure;

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
	void AtomLeaveCell(Atom* a_pLeavingAtom);
	void AtomEnterCell(Atom* a_pEnteringAtom);
	void OnGravityChange();
	//
	std::vector<Mob*> m_MobsInCell;
	std::vector<Atom*> m_AtomsInCell;
	std::vector<Turf*> m_TurfsInCell;
	std::vector<Object*> m_ObjsInCell;
	std::vector<Structure*> m_StructuresInCell;
	//
	std::vector<Atom*> m_AtomsWantingGravUpdates;
private:
	std::vector< std::pair<GravitySource*,float> > m_GravityAffectors;
	//
};

#endif	MAP_CELL_HPP