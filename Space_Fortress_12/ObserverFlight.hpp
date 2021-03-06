#ifndef OBSERVER_FLIGHT_HPP
#define OBSERVER_FLIGHT_HPP

#include "InputModule.hpp"
#include <OGRE\OgreVector3.h>
#include <OGRE\OgreQuaternion.h>

class ObserverFlight
:	public InputModule
{
public:
	ObserverFlight(Mob* a_pOwnedMob);
	virtual void Update(float a_DeltaT);
	//
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );
	//
	Ogre::Vector3 m_RelativeMoveDir;
	float m_FlySpeed;
	//
};

#endif	OBSERVER_FLIGHT_HPP