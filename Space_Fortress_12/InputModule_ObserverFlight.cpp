#include "InputModule_ObserverFlight.hpp"

#include "Mob.hpp"
#include "Client.hpp"

#include <OGRE\OgreSceneNode.h>
#include <OGRE\OgreCamera.h>

ObserverFlight::ObserverFlight(Mob* a_pOwnedMob, Client* a_pOwnedClient)
:	InputModule(a_pOwnedMob, a_pOwnedClient)
,	m_FlySpeed(1.f)
,	m_RelativeMoveDir(Ogre::Vector3(0,0,0))
,	m_LookAngle(Ogre::Vector3(0,0,0))
,	m_LookOrientation(Ogre::Quaternion::IDENTITY)
{
	//
}

void ObserverFlight::Update(float a_DeltaT)
{
	if(!m_pOwnedMob || !m_pOwnedClient)
		return;

	m_pOwnedMob->m_pAtomEntitySceneNode->setPosition(m_pOwnedMob->m_pAtomEntitySceneNode->getPosition() + m_pOwnedClient->m_pCamera->getOrientation() * m_RelativeMoveDir * a_DeltaT * m_FlySpeed);
}

bool ObserverFlight::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return false;
}

bool ObserverFlight::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return false;
}

bool ObserverFlight::keyPressed( const OIS::KeyEvent &arg )
{
	if(!m_pOwnedMob)
		return false;

	switch(arg.key)
	{
	/*case(OIS::KC_P):
		{
			Ogre::Vector3 outPos = m_pOwnedMob->m_pMobSceneNode->getPosition();
			std::cout << outPos.x << ", " << outPos.y << ", " << outPos.z << std::endl;
			break;
		}*/
	/*case(OIS::KC_ESCAPE):
		{
			exit(0);
			break;
		}*/
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

bool ObserverFlight::keyReleased( const OIS::KeyEvent &arg )
{
	if(!m_pOwnedMob)
		return false;

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
			m_RelativeMoveDir.z = -0;
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

bool ObserverFlight::mouseMoved( const OIS::MouseEvent &arg )
{
	if(!m_pOwnedClient || !m_pOwnedClient->m_pCamera)
		return false;

	m_pOwnedClient->m_pCamera->yaw(Ogre::Radian(-float(arg.state.X.rel) / 1000.f));
	m_pOwnedClient->m_pCamera->pitch(Ogre::Radian(-float(arg.state.Y.rel) / 1000.f));

	return true;
}
