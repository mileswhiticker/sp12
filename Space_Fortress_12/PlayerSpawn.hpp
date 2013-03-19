#ifndef PLAYER_SPAWN_HPP
#define PLAYER_SPAWN_HPP

#include <OGRE\OgreVector3.h>

class PlayerSpawn
{
public:
	PlayerSpawn()
	{
		pos = Ogre::Vector3(0,0,0);
		dir = 1;
		type = "default";
	}
	Ogre::Vector3 pos;
	int dir;
	std::string type;
};

#endif	PLAYER_SPAWN_HPP