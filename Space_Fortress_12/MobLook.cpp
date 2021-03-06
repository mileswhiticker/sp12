#include "MobLook.hpp"

#include "Client.hpp"
#include "Mob.hpp"

#include <Ogre\OgreSceneNode.h>
#include <Ogre\OgreCamera.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>
#include "BtOgreHelper.hpp"

MobLook::MobLook(Mob* a_pOwnedMob)
:	InputModule(a_pOwnedMob)
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
	if(m_pOwnedMob->m_pPossessingClient && m_pOwnedMob->m_pPossessingClient->m_pCamera)
	{
		if(true)		//IsOnGround()
		{
			//m_pOwnedMob->m_pPossessingClient->m_pCamera->yaw(Ogre::Radian(-float(arg.state.X.rel) / 1000.f));
			m_pOwnedMob->m_pPossessingClient->m_pCamera->pitch(Ogre::Radian(-float(arg.state.Y.rel) / 1000.f));

			//rotate the model
			if(m_pOwnedMob->m_pPossessingClient->m_pPossessedMob->m_pAtomRootSceneNode && m_pOwnedMob->m_pPossessingClient->m_pPossessedMob->m_pRigidBody)
			{
				//todo: are there are better ways to do this?

				//rotate rigidbody
				/*btTransform worldTransform = m_pOwnedMob->m_pPossessingClient->m_pPossessedMob->m_pRigidBody->getWorldTransform();
				btQuaternion orientation = worldTransform.getRotation();
				btQuaternion rotQuat = btQuaternion(btVector3(0,1,0), -float(arg.state.X.rel) / 1000.f);
				orientation = orientation * rotQuat;
				worldTransform.setRotation(orientation);
				m_pOwnedMob->m_pPossessingClient->m_pPossessedMob->m_pRigidBody->setWorldTransform(worldTransform);*/

				//rotate model
				m_pOwnedMob->m_pPossessingClient->m_pPossessedMob->m_pAtomRootSceneNode->yaw(Ogre::Radian(-float(arg.state.X.rel) / 1000.f));
				btTransform worldTransform = m_pOwnedMob->m_pPossessingClient->m_pPossessedMob->m_pRigidBody->getWorldTransform();
				worldTransform.setRotation( OGRE2BT(m_pOwnedMob->m_pPossessingClient->m_pPossessedMob->m_pAtomRootSceneNode->getOrientation()) );
				m_pOwnedMob->m_pPossessingClient->m_pPossessedMob->m_pRigidBody->setWorldTransform(worldTransform);

				//Ogre::Quaternion cameraOrientation = m_pOwnedMob->m_pPossessingClient->m_pCamera->getOrientation();
				//Ogre::Vector3 lookDir = cameraOrientation * Ogre::Vector3::UNIT_Z;
				//lookDir.y = 0;
				//m_pOwnedMob->m_pPossessingClient->m_pPossessedMob->m_pAtomRootSceneNode->lookAt(lookDir, Ogre::Node::TS_LOCAL);
			}
		}
		else
		{
			//todo: limit the look angles to something like 45 or 90 degrees in a cone, pointing to the direction that the body is actually facing
			//

			//m_pOwnedMob->m_pPossessingClient->m_pCamera->lookAt(m_pOwnedMob->m_pPossessingClient->m_pCamera->getDerivedPosition() + m_pOwnedMob->m_pPossessingClient->m_pPossessedMob->m_pAtomEntitySceneNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z);
		}

		return true;
	}
	return false;
}
