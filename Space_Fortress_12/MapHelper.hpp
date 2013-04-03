#ifndef MAP_HELPER_HPP
#define MAP_HELPER_HPP

#include <OGRE\OgreVector3.h>

std::string GetCoordsString(Ogre::Vector3 a_Coords);
std::string GetCoordsString(int a_X, int a_Y, int a_Z);
Ogre::Vector3 GetCoordsInDir(int a_X, int a_Y, int a_Z, int a_Direction);
Ogre::Vector3 GetCoordsInDir(Ogre::Vector3 a_Coords, int a_Direction);
int ReverseDir(int a_SourceDir);

#endif	MAP_HELPER_HPP