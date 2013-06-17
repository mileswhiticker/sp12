#ifndef INPUTMODULE_GHOSTMOB_HPP
#define INPUTMODULE_GHOSTMOB_HPP

#include "InputModule.hpp"

class GhostMob
:	public InputModule
{
public:
	GhostMob(Mob* a_pOwnedMob, Client* a_pOwnedClient);
	virtual void Update(float a_DeltaT);
	//
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	//
};

#endif	INPUTMODULE_GHOSTMOB_HPP