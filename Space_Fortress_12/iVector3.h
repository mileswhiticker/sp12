#ifndef IVECTOR3_HPP
#define IVECTOR3_HPP

#include <OGRE\OgreVector3.h>

struct iVector3
{
	iVector3(int a_x, int a_y, int a_z)
	:	x(a_x)
	,	y(a_y)
	,	z(a_z)
	{}
	iVector3()
	:	x(0)
	,	y(0)
	,	z(0)
	{}
	unsigned int x;
	unsigned int y;
	unsigned int z;

	Ogre::Vector3 OgreVector3Copy()
	{
		return Ogre::Vector3(Ogre::Real(x), Ogre::Real(y), Ogre::Real(z));
	}
};

#endif	IVECTOR3_HPP