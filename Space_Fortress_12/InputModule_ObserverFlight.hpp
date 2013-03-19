#ifndef OBSERVER_FLIGHT_HPP
#define OBSERVER_FLIGHT_HPP

#include "InputModule.hpp"
#include <OGRE\OgreVector3.h>
#include <OGRE\OgreQuaternion.h>

class ObserverFlight
:	public InputModule
{
public:
	ObserverFlight(Mob* a_pOwnedMob, Client* a_pOwnedClient);
	virtual void Update(float a_DeltaT);
	//
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	//
	Ogre::Vector3 m_RelativeMoveDir;
	Ogre::Vector3 m_LookAngle;
	Ogre::Quaternion m_LookOrientation;
	//
	float m_FlySpeed;
	//
};

#endif	OBSERVER_FLIGHT_HPP