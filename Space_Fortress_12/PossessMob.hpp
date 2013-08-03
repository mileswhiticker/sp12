#ifndef INPUTMODULE_POSSESSMOB_HPP
#define INPUTMODULE_POSSESSMOB_HPP

#include "Component.hpp"

class PossessMob
:	public Component
{
public:
	PossessMob(Mob* a_pOwnedMob, Client* a_pOwnedClient);
	virtual void Update(float a_DeltaT);
	//
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	//
	void SetTargetMob(Mob* a_pTargetPossessMob);
	//
private:
	Mob* m_pTargetPossessMob;
	//
};

#endif	INPUTMODULE_POSSESSMOB_HPP