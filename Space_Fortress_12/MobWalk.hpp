#ifndef INPUTMODULE_MOBWALK_HPP
#define INPUTMODULE_MOBWALK_HPP

#include "InputModule.hpp"
//#include <LinearMath\btVector3.h>
#include <OGRE\OgreVector3.h>

namespace Ogre
{
	class Light;
};

class MobWalk
:	public InputModule
{
public:
	MobWalk(Mob* a_pOwnedMob);
	virtual void Update(float a_DeltaT);
	//
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );
	//
	Ogre::Light* m_pPointLight;
	Ogre::Vector3 m_RelativeMoveDir;
	bool m_IsSprinting;
	float m_WalkSpeed;
	float m_RunSpeed;
};

#endif