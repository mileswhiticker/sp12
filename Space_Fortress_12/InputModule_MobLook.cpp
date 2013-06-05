#include "InputModule_MobLook.hpp"

#include "Client.hpp"
#include "Mob.hpp"

#include <Ogre\OgreSceneNode.h>
#include <Ogre\OgreCamera.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>

MobLook::MobLook(Mob* a_pOwnedMob, Client* a_pOwnedClient)
:	InputModule(a_pOwnedMob, a_pOwnedClient)
,	m_LookAngle(Ogre::Vector3(0,0,0))
,	m_LookOrientation(Ogre::Quaternion::IDENTITY)
{
	//
}

void MobLook::Update(float a_DeltaT)
{
	//
}

bool MobLook::mouseMoved( const OIS::MouseEvent &arg )
{
	if(m_pOwnedClient && m_pOwnedClient->m_pCamera)
	{
		if(true)		//IsOnGround()
		{
			m_pOwnedClient->m_pCamera->yaw(Ogre::Radian(-float(arg.state.X.rel) / 1000.f));
			m_pOwnedClient->m_pCamera->pitch(Ogre::Radian(-float(arg.state.Y.rel) / 1000.f));

			//rotate the model
			if(m_pOwnedClient->m_pPossessedMob->m_pAtomEntitySceneNode)
			{
				//todo: are there are better ways to do this?
				m_pOwnedClient->m_pPossessedMob->m_pAtomEntitySceneNode->yaw(Ogre::Radian(-float(arg.state.X.rel) / 1000.f));

				//Ogre::Quaternion cameraOrientation = m_pOwnedClient->m_pCamera->getOrientation();
				//Ogre::Vector3 lookDir = cameraOrientation * Ogre::Vector3::UNIT_Z;
				//lookDir.y = 0;
				//m_pOwnedClient->m_pPossessedMob->m_pAtomRootSceneNode->lookAt(lookDir, Ogre::Node::TS_LOCAL);
			}
		}
		else
		{
			//todo: limit the look angles to something like 45 or 90 degrees in a cone, pointing to the direction that the body is actually facing
			//

			//m_pOwnedClient->m_pCamera->lookAt(m_pOwnedClient->m_pCamera->getDerivedPosition() + m_pOwnedClient->m_pPossessedMob->m_pAtomEntitySceneNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z);
		}

		return true;
	}
	return false;
}
