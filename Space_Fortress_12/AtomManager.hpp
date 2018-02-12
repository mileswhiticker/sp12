#ifndef ATOM_MANAGER_HPP
#define ATOM_MANAGER_HPP

#include <set>
#include <string>
#include <OGRE\OgreVector3.h>
#include "BuildFlags.hpp"

class Atom;
class Object;
class Structure;
class MapCell;
class Mob;
class Turf;
class Context;

class AtomManager
{
public:
	static AtomManager& GetSingleton()
	{
		static AtomManager& instance = *new AtomManager();
		return instance;
	}
	static AtomManager* GetSingletonPtr()
	{
		return &AtomManager::GetSingleton();
	}
	void Update(float a_DeltaT);
	
	//the first 6 bits of a_AdditionalFlags are directions
	Object* CreateObject(int a_ObjectType, Ogre::Vector3 a_Pos, int a_AdditionalFlags = INSTANTIATE_IMMEDIATELY);
	Structure* CreateStructure(int a_StructureType, Turf* a_pLocTurf, int a_AdditionalFlags = INSTANTIATE_IMMEDIATELY);
	Mob* CreateMob(int a_MobType, Ogre::Vector3 a_SpawnPos, int a_AdditionalFlags = INSTANTIATE_IMMEDIATELY);
	//
	Context* CreateInteractContext(int a_Dir, int a_Quadrant, int a_Layer);
	//
	
	void SetTurfWantingUpdate(Turf* a_pTurfToUpdate);
	void StopTurfWantingUpdate(Turf* a_pTurfToStopUpdate);

	void DeleteStructure(Structure* a_pStructureToDel);
	void DeleteMob(Mob* a_pMobToDel);

	std::set<Structure*> m_StructuresInWorld;
	std::set<Mob*> m_MobsInWorld;
	std::set<Atom*> m_ObjectsInWorld;
	std::map<int,Turf*> m_TurfsInWorldWantingUpdate;
	//
private:
	AtomManager();
	bool cellsFlashing;
	//
};

#endif	ATOM_MANAGER_HPP