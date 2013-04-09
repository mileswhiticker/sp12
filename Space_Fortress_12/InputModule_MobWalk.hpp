#ifndef INPUTMODULE_MOBWALK_HPP
#define INPUTMODULE_MOBWALK_HPP

#include "InputModule.hpp"
#include <OGRE\OgreVector3.h>
#include <OGRE\OgreQuaternion.h>

namespace Ogre
{
	class Light;
};

class MobWalk
:	public InputModule
{
public:
	MobWalk(Mob* a_pOwnedMob, Client* a_pOwnedClient);
	virtual void Update(float a_DeltaT);
	//
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );
	//
	Ogre::Light* m_pPointLight;
};

#endif