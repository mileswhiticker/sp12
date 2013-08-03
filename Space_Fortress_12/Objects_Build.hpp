#ifndef OBJECTS_BUILD_HPP
#define OBJECTS_BUILD_HPP

#include "Object.hpp"
#include <OGRE/OgreVector3.h>

class Turf;
namespace Ogre
{
	class Entity;
};

class GirderObject
:	public Object
{
public:
	GirderObject(Ogre::Vector3 a_Position);
	virtual void Build(Turf* a_pLocTurf, int a_Direction, int a_Layer);
	virtual void BuildHighlight(Turf* a_pLocTurf, int a_Direction, int a_Layer);
	//
protected:
	Ogre::Entity* m_pHighlightMesh;
	Turf* m_pHighlightTurf;
	int m_HighlightDir;
	int m_HighlightLayer;
	//
};

#endif OBJECTS_BUILD_HPP