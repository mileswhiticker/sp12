#ifndef INPUTMODULE_MOUSELOOK
#define INPUTMODULE_MOUSELOOK

#include "InputModule.hpp"

class MouseLook
:	public InputModule
{
public:
	MouseLook(Mob* a_pOwnedMob, Client* a_pOwnedClient);
	virtual void Update(float a_DeltaT);
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	//
	Ogre::Vector3 m_LookAngle;
	Ogre::Quaternion m_LookOrientation;
	//
private:
	//
};

#endif	INPUTMODULE_MOUSELOOK