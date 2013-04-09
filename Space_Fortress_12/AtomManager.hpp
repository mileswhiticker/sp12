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
	Object* CreateObject(int a_ObjectType, Ogre::Vector3 a_Pos, Atom** a_ppAtomLocation = NULL, int a_AdditionalFlags = INSTANTIATE_IMMEDIATELY);
	Structure* CreateStructure(int a_StructureType, MapCell* a_pLocMapCell, Structure** a_ppAtomLocation = NULL, int a_AdditionalFlags = INSTANTIATE_IMMEDIATELY);
	Mob* CreateMob(int a_MobType, Ogre::Vector3 a_SpawnPos, Mob** a_ppAtomLocation = NULL, int a_AdditionalFlags = INSTANTIATE_IMMEDIATELY);
	//
	void DeleteStructure(Structure* a_pStructureToDel);
	void AtomManager::DeleteMob(Mob* a_pMobToDel);

	void ToggleCellFlashing();
	//
private:
	AtomManager();
	bool cellsFlashing;
	//
	std::set<Structure*> m_StructuresInWorld;
	std::set<Mob*> m_MobsInWorld;
	std::set<Atom*> m_ObjectsInWorld;
	//
};

#endif	ATOM_MANAGER_HPP