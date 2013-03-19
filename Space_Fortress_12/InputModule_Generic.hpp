#ifndef INPUTMODULE_GENERIC_HPP
#define INPUTMODULE_GENERIC_HPP

#include "InputModule.hpp"
#include <OGRE\OgreVector3.h>
#include <OGRE\OgreQuaternion.h>

class Generic
:	public InputModule
{
public:
	Generic(Mob* a_pOwnedMob, Client* a_pOwnedClient);
	virtual void Update(float a_DeltaT);
	//
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	//
};

#endif	INPUTMODULE_GENERIC_HPP