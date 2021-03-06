#ifndef PLAYER_GENERIC_HPP
#define PLAYER_GENERIC_HPP

#include "InputModule.hpp"
#include <OGRE\OgreVector3.h>
#include <OGRE\OgreQuaternion.h>

namespace Ogre
{
	class Light;
};

class PlayerGeneric
:	public InputModule
{
public:
	PlayerGeneric(Mob* a_pOwnedMob);
	virtual void Update(float a_DeltaT);
	//
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	//
	Ogre::Light* m_pPointLight;
};

#endif	PLAYER_GENERIC_HPP