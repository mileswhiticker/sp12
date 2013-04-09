#ifndef CACHED_HPP
#define CACHED_HPP

#include <OGRE\OgreColourValue.h>
#include <OGRE\OgreVector3.h>

struct CachedLine
{
	CachedLine(Ogre::Vector3 a_startPos, Ogre::Vector3 a_endPos, Ogre::ColourValue a_colour)
	{
		startPos = a_startPos;
		endPos = a_endPos;
		colour = a_colour;
	}
	Ogre::Vector3 startPos;
	Ogre::Vector3 endPos;
	Ogre::ColourValue colour;
};

struct CachedSphere
{
	CachedSphere(Ogre::Vector3 a_pos, float a_radius, Ogre::ColourValue a_colour)
	{
		pos = a_pos;
		radius = a_radius;
		colour = a_colour;
	}
	Ogre::Vector3 pos;
	float radius;
	Ogre::ColourValue colour;
};

struct CachedCube
{
	CachedCube(Ogre::Vector3 a_pos, Ogre::Vector3 a_Verts[8], Ogre::ColourValue a_colour)
	{
		pos = a_pos;
		for(int i=0;i<8;++i)
			m_Verts[i] = a_Verts[i];
		colour = a_colour;
	}
	Ogre::Vector3 pos;
	Ogre::Vector3 m_Verts[8];
	Ogre::ColourValue colour;
};

#endif	CACHED_HPP