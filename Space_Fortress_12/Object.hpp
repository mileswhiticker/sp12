#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "Atom.hpp"

#include <OGRE\OgreVector3.h>

class Turf;
class Mob;
class InputModule;
namespace Ogre
{
	class SceneNode;
};

class Object
:	public Atom
{
public:
	enum ObjType
	{
		OBJTYPE_UNKNOWN = 0,
		//
		BOX,
		BUILDABLE,
		//
		OBJTYPE_MAXVAL
	};
	Object(Ogre::Vector3 a_StartPos, int a_StartDirection = 0);
	virtual void InstantiateAtom();
	virtual void Update(float a_DeltaT);
	ObjType GetObjType();
	//
	virtual void Build(Turf* a_pLocTurf, int a_Direction, int a_Layer);

	//mob interaction
	InputModule* GetEquipInputModule();
	//
	virtual bool AddToOtherAtomContents(Atom* a_pAtom);
	virtual bool RemoveFromOtherAtomContents();
	//
protected:
	ObjType m_MyObjType;
	std::string m_MeshName;
	Mob* m_pHoldingMob;

	//interaction stuff
	/*Atom::AtomType m_TargetAtomType;
	int m_TargetAtomSubtype;*/
	InputModule* m_pEquipInputModule;
	//
};

#endif	OBJECT_HPP