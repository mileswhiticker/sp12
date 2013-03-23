#ifndef ATOM_MANAGER_HPP
#define ATOM_MANAGER_HPP

#include <string>
#include <OGRE\OgreVector3.h>

class Atom;
class Object;
class Girder;
class Structure;

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
	Atom* CreateAtom(int a_AtomType, Ogre::Vector3 a_Pos, bool a_InstantiateImmediately = true, Atom** a_ppAtomLocation = NULL);
	Atom* CreateAtomBuildpoint(int a_AtomType, Ogre::Vector3 a_Pos, bool a_InstantiateImmediately = true, Atom** a_ppAtomLocation = NULL);
	Structure* CreateStructure(int a_StructureType, Ogre::Vector3 a_Pos, bool a_InstantiateImmediately = true, Structure** a_ppAtomLocation = NULL);
	Structure* CreateStructureBuildpoint(int a_StructureType, Ogre::Vector3 a_Pos, bool a_InstantiateImmediately = true, Structure** a_ppAtomLocation = NULL);
	//bool CreateObject(std::string a_TypeTag, Ogre::Vector3 a_Position);
	//Cell* CreateCell(Ogre::SceneNode* a_pSceneNode, Ogre::Vector3 a_Pos, std::string a_SkeletonType);
	void Update(float a_DeltaT);
	//
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