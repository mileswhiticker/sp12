#ifndef INPUTMODULE_MOBLOOK
#define INPUTMODULE_MOBLOOK

#include "InputModule.hpp"

class MobLook
:	public InputModule
{
public:
	MobLook(Mob* a_pOwnedMob);
	virtual void Update(float a_DeltaT);
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	//
	Ogre::Vector3 m_LookAngle;
	Ogre::Quaternion m_LookOrientation;
	//
private:
	//
};

#endif	INPUTMODULE_MOBLOOK