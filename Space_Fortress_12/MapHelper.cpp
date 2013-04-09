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

int ReverseDir(int a_SourceDir)
{
	if(a_SourceDir & NORTH)
	{
		a_SourceDir &= ~NORTH;
		a_SourceDir |= SOUTH;
	}
	else if(a_SourceDir & SOUTH)
	{
		a_SourceDir &= ~SOUTH;
		a_SourceDir |= NORTH;
	}
	if(a_SourceDir & EAST)
	{
		a_SourceDir &= ~EAST;
		a_SourceDir |= WEST;
	}
	else if(a_SourceDir & WEST)
	{
		a_SourceDir &= ~WEST;
		a_SourceDir |= EAST;
	}
	if(a_SourceDir & UP)
	{
		a_SourceDir &= ~UP;
		a_SourceDir |= DOWN;
	}
	else if(a_SourceDir & DOWN)
	{
		a_SourceDir &= ~DOWN;
		a_SourceDir |= UP;
	}
	return a_SourceDir;
}

Ogre::Vector3 GetUnitVectorFromDir(int a_Direction)
{
	Ogre::Vector3 outVector = Ogre::Vector3::ZERO;
	if(a_Direction & NORTH)
	{
		outVector += Ogre::Vector3::UNIT_Z;
	}
	if(a_Direction & SOUTH)
	{
		outVector += Ogre::Vector3::NEGATIVE_UNIT_Z;
	}
	if(a_Direction & EAST)
	{
		outVector += Ogre::Vector3::UNIT_X;
	}
	if(a_Direction & WEST)
	{
		outVector += Ogre::Vector3::NEGATIVE_UNIT_X;
	}
	if(a_Direction & UP)
	{
		outVector += Ogre::Vector3::UNIT_Y;
	}
	if(a_Direction & DOWN)
	{
		outVector += Ogre::Vector3::NEGATIVE_UNIT_Y;
	}
	outVector.normalise();
	return outVector;
}
