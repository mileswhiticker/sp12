#include "ObserverFlight.hpp"

#include "Mob.hpp"
#include "Client.hpp"

#include <OGRE\OgreSceneNode.h>
#include <OGRE\OgreCamera.h>

ObserverFlight::ObserverFlight(Mob* a_pOwnedMob)
:	InputModule(a_pOwnedMob)
,	m_FlySpeed(1.f)
,	m_RelativeMoveDir(Ogre::Vector3(0,0,0))
{
	//
}

void ObserverFlight::Update(float a_DeltaT)
{
	if(m_RelativeMoveDir.squaredLength())
	{
		Ogre::Quaternion moveOrientation = Ogre::Quaternion::IDENTITY;
		if(m_pOwnedMob->m_pPossessingClient && m_pOwnedMob->m_pPossessingClient->m_pCamera)
			moveOrientation = m_pOwnedMob->m_pPossessingClient->m_pCamera->getOrientation();
		//
		Ogre::Vector3 oldPos = Ogre::Vector3::ZERO;
		if(m_pOwnedMob && m_pOwnedMob->m_pAtomRootSceneNode)
			oldPos = m_pOwnedMob->m_pAtomRootSceneNode->getPosition();
		m_pOwnedMob->m_pAtomRootSceneNode->setPosition(oldPos + moveOrientation * m_RelativeMoveDir * a_DeltaT * m_FlySpeed);
	}
}

bool ObserverFlight::keyPressed( const OIS::KeyEvent &arg )
{
	if(m_pOwnedMob)
	{
		switch(arg.key)
		{
		case(OIS::KC_LSHIFT):
			{
				m_FlySpeed = 6.f;
				break;
			}
		case(OIS::KC_W):
			{
				m_RelativeMoveDir.z = -1;
				m_RelativeMoveDir.normalise();
				break;
			}
		case(OIS::KC_S):
			{
				m_RelativeMoveDir.z = 1;
				m_RelativeMoveDir.normalise();
				break;
			}
		case(OIS::KC_A):
			{
				m_RelativeMoveDir.x = -1;
				m_RelativeMoveDir.normalise();
				break;
			}
		case(OIS::KC_D):
			{
				m_RelativeMoveDir.x = 1;
				m_RelativeMoveDir.normalise();
				break;
			}
		case(OIS::KC_SPACE):
			{
				m_RelativeMoveDir.y = 1;
				m_RelativeMoveDir.normalise();
				break;
			}
		case(OIS::KC_LCONTROL):
			{
				m_RelativeMoveDir.y = -1;
				m_RelativeMoveDir.normalise();
				break;
			}
		}
		return true;
	}
	return false;
}

bool ObserverFlight::keyReleased( const OIS::KeyEvent &arg )
{
	if(m_pOwnedMob)
	{
		switch(arg.key)
		{
		case(OIS::KC_LSHIFT):
			{
				m_FlySpeed = 1.f;
				break;
			}
		case(OIS::KC_W):
			{
				m_RelativeMoveDir.z = 0;
				m_RelativeMoveDir.normalise();
				break;
			}
		case(OIS::KC_S):
			{
				m_RelativeMoveDir.z = -0;	//legit pro
				m_RelativeMoveDir.normalise();
				break;
			}
		case(OIS::KC_A):
			{
				m_RelativeMoveDir.x = -0;
				m_RelativeMoveDir.normalise();
				break;
			}
		case(OIS::KC_D):
			{
				m_RelativeMoveDir.x = 0;
				m_RelativeMoveDir.normalise();
				break;
			}
		case(OIS::KC_SPACE):
			{
				m_RelativeMoveDir.y = 0;
				m_RelativeMoveDir.normalise();
				break;
			}
		case(OIS::KC_LCONTROL):
			{
				m_RelativeMoveDir.y = 0;
				m_RelativeMoveDir.normalise();
				break;
			}
		}
		return true;
	}
	return false;
}
