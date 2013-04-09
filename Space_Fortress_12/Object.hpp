#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "Atom.hpp"

#include <OGRE\OgreVector3.h>
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
		BOX
	};
	Object(Ogre::Vector3 a_StartPos, int a_StartDirection = 0);
	virtual void InstantiateAtom();
	virtual void Update(float a_DeltaT);
	ObjType GetObjType();
	//
protected:
	ObjType m_MyObjType;
	//
};

class Box
:	public Object
{
public:
	Box(Ogre::Vector3 a_Position);
	virtual void InstantiateAtom();
	//
protected:
	//
};

#endif	OBJECT_HPP