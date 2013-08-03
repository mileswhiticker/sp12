#ifndef INPUTMODULE_MobGhost_HPP
#define INPUTMODULE_MobGhost_HPP

#include "Component.hpp"

class MobGhost
:	public Component
{
public:
	MobGhost(Mob* a_pOwnedMob, Client* a_pOwnedClient);
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