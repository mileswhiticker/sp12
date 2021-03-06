#ifndef INPUTMODULE_OBSERVERLOOK
#define INPUTMODULE_OBSERVERLOOK

#include "InputModule.hpp"

class ObserverLook
:	public InputModule
{
public:
	ObserverLook(Mob* a_pOwnedMob);
	virtual void Update(float a_DeltaT);
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	//
	Ogre::Vector3 m_LookAngle;
	Ogre::Quaternion m_LookOrientation;
	//
private:
	//
};

#endif	INPUTMODULE_OBSERVERLOOK