#ifndef INPUTMODULE_MobGhost_HPP
#define INPUTMODULE_MobGhost_HPP

#include "InputModule.hpp"

class MobGhost
:	public InputModule
{
public:
	MobGhost(Mob* a_pOwnedMob);
	virtual void Update(float a_DeltaT);
	//
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	//
};

#endif	INPUTMODULE_MobGhost_HPP