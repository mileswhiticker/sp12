#ifndef GRAVITY_SOURCE_HPP
#define GRAVITY_SOURCE_HPP

#include <Ogre\OgreVector3.h>

class GravitySource
{
public:
	GravitySource();
	Ogre::Vector3 GetGravity();
	//
protected:
	Ogre::Vector3 m_Gravity;
	bool m_Enabled;
	//
};

#endif	GRAVITY_SOURCE_HPP