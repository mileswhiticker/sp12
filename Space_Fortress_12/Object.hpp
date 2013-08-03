#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "Atom.hpp"

#include <OGRE\OgreVector3.h>

class Turf;
class Mob;
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
		UNKNOWN = 0,
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
	//
	void AddToMobInventory(Mob* a_pMob);
	void RemoveFromMobInventory();
	//
protected:
	ObjType m_MyObjType;
	std::string m_MeshName;
	Mob* m_pHoldingMob;
	//
};

class Box
:	public Object
{
public:
	Box(Ogre::Vector3 a_Position);
	//virtual void InstantiateAtom();
	//
protected:
	//
};

#endif	OBJECT_HPP