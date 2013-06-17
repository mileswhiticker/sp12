#ifndef ATOM_MANAGER_HPP
#define ATOM_MANAGER_HPP

#include <string>
#include <OGRE\OgreVector3.h>
#include <set>

class Atom;
class Object;
class Girder;
class Structure;
class MapCell;
class Mob;
class Turf;

#define INSTANTIATE_IMMEDIATELY 64
#define BUILD_POINT 128

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
	Turf* CreateTurf(int a_TurfType, MapCell* a_pLocMapCell, int a_AdditionalFlags = INSTANTIATE_IMMEDIATELY);
	Mob* CreateMob(int a_MobType, Ogre::Vector3 a_SpawnPos, int a_AdditionalFlags = INSTANTIATE_IMMEDIATELY);
	//
	void ClearMapCell(MapCell* a_pMapCell);
	//
	void DeleteStructure(Structure* a_pStructureToDel);
	void DeleteMob(Mob* a_pMobToDel);
	void DeleteTurf(Turf* a_pTurfToDel);

	std::set<Structure*> m_StructuresInWorld;
	std::set<Mob*> m_MobsInWorld;
	std::set<Atom*> m_ObjectsInWorld;
	std::set<Turf*> m_TurfsInWorld;
	//
private:
	AtomManager();
	bool cellsFlashing;
	//
};

#endif	ATOM_MANAGER_HPP