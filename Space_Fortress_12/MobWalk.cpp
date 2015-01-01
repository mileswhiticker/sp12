#include "MobWalk.hpp"

#include "Mob.hpp"
#include "Client.hpp"

#include "BtOgreHelper.hpp"
#include <OGRE\OgreCamera.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>
#include <OGRE\OgreSceneNode.h>

MobWalk::MobWalk(Mob* a_pOwnedMob)
:	InputModule(a_pOwnedMob)
,	m_RelativeMoveDir(Ogre::Vector3::ZERO)
,	m_IsSprinting(false)
,	m_WalkSpeed(1.f)
,	m_RunSpeed(2.f)
{
	//
}

void MobWalk::Update(float a_DeltaT)
{
	if(m_pOwnedMob && m_pOwnedMob->m_pRigidBody && m_pOwnedMob->IsOnGround() && m_RelativeMoveDir.squaredLength())
	{
		//grab the direction the body is facing
		Ogre::Quaternion moveOrientation = m_pOwnedMob->m_pAtomRootSceneNode->getOrientation();
		btTransform worldTransform = m_pOwnedMob->m_pRigidBody->getWorldTransform();
		btVector3 worldPos = worldTransform.getOrigin();

		//walking or running?
		float moveSpeed = m_WalkSpeed;
		if(m_IsSprinting)
		{
			moveSpeed = m_RunSpeed;
		}

		//move the transform forward
		worldPos += OGRE2BT(moveOrientation * m_RelativeMoveDir * (a_DeltaT * moveSpeed));
		worldTransform.setOrigin(worldPos);
		m_pOwnedMob->m_pRigidBody->setWorldTransform(worldTransform);

		//check if we're still on the "ground"
		if(!m_pOwnedMob->UpdateOnGround())
		{
			//we walked off the edge, float away
			m_pOwnedMob->m_pRigidBody->activate(true);
			m_pOwnedMob->m_pRigidBody->setLinearVelocity(OGRE2BT(moveOrientation * m_RelativeMoveDir) * moveSpeed);
			m_pOwnedMob->m_pRigidBody->setGravity(btVector3(0,0,0));
		}
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
			if(m_pOwnedMob && m_pOwnedMob->m_pRigidBody && m_pOwnedMob->IsOnGround())
			{
				//jump
				m_pOwnedMob->m_IsOnGround = false;
				m_pOwnedMob->m_pRigidBody->activate(true);
				//Ogre::Vector3 force(0, (1.0f / m_pOwnedMob->m_pRigidBody->getInvMass()) * 3, 0);
				//m_pOwnedMob->m_pRigidBody->applyCentralImpulse( OGRE2BT(force) );
				btVector3 gravityDir = m_pOwnedMob->m_pRigidBody->getGravity().normalized();
				m_pOwnedMob->m_pRigidBody->setLinearVelocity(-gravityDir * 2);
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
