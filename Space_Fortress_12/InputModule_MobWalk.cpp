#include "InputModule_MobWalk.hpp"

#include "Mob.hpp"
#include "Client.hpp"

#include "BtOgreHelper.hpp"
#include <OGRE\OgreCamera.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>
#include <OGRE\OgreSceneNode.h>

MobWalk::MobWalk(Mob* a_pOwnedMob, Client* a_pOwnedClient)
:	InputModule(a_pOwnedMob, a_pOwnedClient)
,	m_RelativeMoveDir(Ogre::Vector3::ZERO)
,	m_IsSprinting(false)
,	m_WalkSpeed(1.f)
,	m_RunSpeed(2.f)
{
	//
}

void MobWalk::Update(float a_DeltaT)
{
	//m_pOwnedMob->m_pRigidBody->wantsSleeping()
	if(m_pOwnedMob && m_pOwnedMob->m_pRigidBody && m_pOwnedMob->IsOnGround())
	{
		/*
		Ogre::Quaternion moveOrientation = Ogre::Quaternion::IDENTITY;
		if(m_pOwnedClient && m_pOwnedClient->m_pCamera)
			moveOrientation = m_pOwnedClient->m_pCamera->getOrientation();
		//
		Ogre::Vector3 oldPos = Ogre::Vector3::ZERO;
		if(m_pOwnedMob && m_pOwnedMob->m_pAtomRootSceneNode)
			oldPos = m_pOwnedMob->m_pAtomRootSceneNode->getPosition();
		m_pOwnedMob->m_pAtomRootSceneNode->setPosition(oldPos + moveOrientation * m_RelativeMoveDir * a_DeltaT * m_FlySpeed);
		*/
		Ogre::Quaternion moveOrientation = m_pOwnedMob->m_pAtomEntitySceneNode->getOrientation();
		/*if(m_pOwnedMob)
		{
			//grab the direction the player is looking
			moveOrientation = m_pOwnedClient->m_pCamera->getOrientation();

			//turn into a vector to remove the y component (there's probably a better way of doing this)
			Ogre::Vector3 lookDir = moveOrientation * Ogre::Vector3::UNIT_Z;
			lookDir.y = 0;
			//if this isn't normalised, i'm pretty sure that won't be reflected in the quaternion

			//turn back into a quaternion
			moveOrientation = Ogre::Vector3::UNIT_Z.getRotationTo(lookDir);
		}*/

		btTransform worldTransform = m_pOwnedMob->m_pRigidBody->getWorldTransform();
		btVector3 worldPos = worldTransform.getOrigin();
		float moveSpeed = m_WalkSpeed;
		if(m_IsSprinting)
		{
			moveSpeed = m_RunSpeed;
		}
		worldPos += OGRE2BT(moveOrientation * m_RelativeMoveDir * (a_DeltaT * moveSpeed));
		worldTransform.setOrigin(worldPos);
		m_pOwnedMob->m_pRigidBody->setWorldTransform(worldTransform);
	}
}

//using keyPress and keyRelease is a bad way to handle input control for movement
//i'll change this later, it needs a more robust system setup
bool MobWalk::keyPressed( const OIS::KeyEvent &arg )
{
	switch(arg.key)
	{
	case(OIS::KC_W):
		{
			m_RelativeMoveDir.z = -1;
			break;
		}
	case(OIS::KC_A):
		{
			m_RelativeMoveDir.x = -1;
			break;
		}
	case(OIS::KC_S):
		{
			m_RelativeMoveDir.z = 1;
			break;
		}
	case(OIS::KC_D):
		{
			m_RelativeMoveDir.x = 1;
			break;
		}
	case(OIS::KC_LSHIFT):
		{
			m_IsSprinting = true;
			break;
		}
	case(OIS::KC_SPACE):
		{
			if(m_pOwnedMob->m_pRigidBody && m_pOwnedMob->IsOnGround())
			{
				//jump
				m_pOwnedMob->m_pRigidBody->activate(true);
				Ogre::Vector3 force(0, (1.0f / m_pOwnedMob->m_pRigidBody->getInvMass()) * 3, 0);
				//m_pOwnedMob->m_pRigidBody->applyCentralImpulse( OGRE2BT(force) );
				m_pOwnedMob->m_pRigidBody->setLinearVelocity(btVector3(0, 2, 0));
			}
			break;
		}
	}
	return true;
}

bool MobWalk::keyReleased( const OIS::KeyEvent &arg )
{
	switch(arg.key)
	{
	case(OIS::KC_W):
		{
			m_RelativeMoveDir.z = 0;
			break;
		}
	case(OIS::KC_A):
		{
			m_RelativeMoveDir.x = 0;
			break;
		}
	case(OIS::KC_S):
		{
			m_RelativeMoveDir.z = 0;
			break;
		}
	case(OIS::KC_D):
		{
			m_RelativeMoveDir.x = 0;
			break;
		}
	case(OIS::KC_LSHIFT):
		{
			m_IsSprinting = false;
			break;
		}
	}
	return true;
}
