#ifndef ATOM_MANAGER_HPP
#define ATOM_MANAGER_HPP

#include <string>
#include <OGRE\OgreVector3.h>

class Atom;
class Object;
class Girder;
class Structure;
class MapCell;

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

	Atom* CreateAtom(int a_AtomType, Ogre::Vector3 a_Pos, bool a_InstantiateImmediately = true, Atom** a_ppAtomLocation = NULL);

	//the first 6 bits of a_AdditionalFlags are directions
	Structure* CreateStructure(int a_StructureType, MapCell* a_pLocMapCell, Structure** a_ppAtomLocation = NULL, int a_AdditionalFlags = INSTANTIATE_IMMEDIATELY);

	void ToggleCellFlashing();
	//
private:
	AtomManager();
	bool cellsFlashing;
	//
	std::vector<Atom*> m_AtomsInWorld;
	std::vector<Girder*> m_GirdersInWorld;
	std::vector<Object*> m_ObjectsInWorld;
	//
};

#endif	ATOM_MANAGER_HPP