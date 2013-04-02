#include "MapHelper.hpp"
#include "Direction.h"

std::string GetCoordsString(Ogre::Vector3 a_Coords)
{
	std::stringstream out;
	out << a_Coords.x << a_Coords.y << a_Coords.z;
	return out.str();
}

std::string GetCoordsString(int a_X, int a_Y, int a_Z)
{
	std::stringstream out;
	out << a_X << a_Y << a_Z;
	return out.str();
}

Ogre::Vector3 GetCoordsInDir(int a_X, int a_Y, int a_Z, int a_Direction)
{
	return GetCoordsInDir(Ogre::Vector3(a_X, a_Y, a_Z), a_Direction);
}

Ogre::Vector3 GetCoordsInDir(Ogre::Vector3 a_Coords, int a_Direction)
{
	if(a_Direction & NORTH)
	{
		a_Coords += Ogre::Vector3::UNIT_Z;
	}
	if(a_Direction & SOUTH)
	{
		a_Coords += Ogre::Vector3::NEGATIVE_UNIT_Z;
	}
	if(a_Direction & EAST)
	{
		a_Coords += Ogre::Vector3::UNIT_X;
	}
	if(a_Direction & WEST)
	{
		a_Coords += Ogre::Vector3::NEGATIVE_UNIT_X;
	}
	if(a_Direction & UP)
	{
		a_Coords += Ogre::Vector3::UNIT_Y;
	}
	if(a_Direction & DOWN)
	{
		a_Coords += Ogre::Vector3::NEGATIVE_UNIT_Y;
	}
	return a_Coords;
}
