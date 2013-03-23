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
	Object();
	virtual void Update(float a_DeltaT);
	//
protected:
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