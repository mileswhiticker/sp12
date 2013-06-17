#include "Mob.hpp"

#include "InputModule.hpp"
#include "Client.hpp"
#include "MapSuite.hpp"
#include "MapCell.hpp"
#include "Application.hpp"

#include <LinearMath\btVector3.h>
#include <BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h>
#include <BulletCollision\CollisionDispatch\btCollisionWorld.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>

#include "CollisionDefines.h"
#include "OgreHelper.hpp"
#include "RandHelper.h"
#include "BtOgreHelper.hpp"

Mob::Mob(Ogre::Vector3 a_StartPos, int a_Direction)
:	Atom(a_StartPos, a_Direction)
,	m_pPossessingClient(NULL)
,	m_MyMobType(UNKNOWN)
,	m_tLeftUprightOrientation(0)
,	m_TargetStandingOrientation(btQuaternion(0,1,0,0))
,	m_tleftNextGroundRaycast(0.5f)
,	m_IsOnGround(false)
,	m_CameraModelOffset(Ogre::Vector3::ZERO)
,	m_Intent(0)
{
	m_MyAtomType = Atom::MOB;
	//btQuaternion(btVector3(0,1,0), 0);
}

Mob::~Mob()
{
	//todo:
	//clear input modules
	//clear bullet rigidbodies, collision shapes
	//clear ogre entities, scenenodes
}

void Mob::Update(float a_DeltaT)
{
	Atom::Update(a_DeltaT);
	for(auto it = m_InputModules.begin(); it != m_InputModules.end(); ++it)
	{
		(*it)->Update(a_DeltaT);
	}

	//grab an updated direction for gravity to orient ourselves against
	if(m_pSourceMapCell)
	{
		//todo: remove the normalise here
		Ogre::Vector3 gravityDir = m_pSourceMapCell->GetGravity().normalisedCopy();
		m_TargetStandingOrientation = OGRE2BT(Ogre::Vector3::NEGATIVE_UNIT_Y.getRotationTo(gravityDir));

		/*Ogre::Quaternion targetOrientation;
		targetOrientation.FromAngleAxis(Ogre::Degree(360), gravityDir);
		m_TargetStandingOrientation = OGRE2BT(targetOrientation);
		m_TargetStandingOrientation = btQuaternion(OGRE2BT(gravityDir), 0);*/
	}

	//if we're "falling", raycast down to see if we're on the "ground"
	m_tleftNextGroundRaycast -= a_DeltaT;
	if(m_UsesGravity && m_pRigidBody->getGravity().length2() > 0 && m_tleftNextGroundRaycast <= 0)
	{
		//todo: this whole method is a little crude
		m_tleftNextGroundRaycast = 0.5f;
		UpdateOnGround();

	}

	//orient physbody to make is upright relative to the direction of gravity
	if(m_pRigidBody && m_tLeftUprightOrientation < 1)
	{
		//just instantly snap to the right direction for now
		//m_tLeftUprightOrientation += a_DeltaT;

		btTransform currentTransform = m_pRigidBody->getWorldTransform();
		btQuaternion currentOrientation = currentTransform.getRotation();

		//target orientation: standing up
		//btQuaternion targetOrientation = btQuaternion(btVector3(0,1,0), 0);
		//currentOrientation = currentOrientation.slerp(m_TargetStandingOrientation, a_DeltaT * 2);
		//currentOrientation = m_TargetStandingOrientation;

		//apply the changes
		//there is a random crash on this line when starting up, sometimes
		currentTransform.setRotation(currentOrientation);
		m_pRigidBody->setWorldTransform(currentTransform);
	}
}

bool Mob::ConnectClient(Client* a_pNewClient)
{
	if(!m_pPossessingClient)
	{
		m_pPossessingClient = a_pNewClient;
		m_pPossessingClient->m_pPossessedMob = this;
		m_pPossessingClient->ResetCamera();

		RegisterKeyListener(this);
		RegisterMouseListener(this);
		
		for(auto it = m_InputModules.begin(); it != m_InputModules.end(); ++it)
		{
			(*it)->SetClient(a_pNewClient);
		}

		return true;
	}
	return false;
}

void Mob::InstantiateAtom()
{
	if(m_pRigidBody)
	{
		m_pRigidBody->applyForce(btVector3(btScalar(fRand(-2,2)), btScalar(fRand(-2,2)), btScalar(fRand(-2,2))), btVector3(fRand(-0.1f,0.1f), fRand(-0.1f,0.1f), fRand(-0.1f,0.1f)));
	}
}

bool Mob::DisconnectClient()
{
	if(m_pPossessingClient)
	{
		m_pPossessingClient = NULL;

		for(auto it = m_InputModules.begin(); it != m_InputModules.end(); ++it)
		{
			(*it)->SetClient(NULL);
		}

		return true;
	}
	return false;
}

bool Mob::keyPressed( const OIS::KeyEvent &arg )
{
	for(auto it = m_InputModules.begin(); it != m_InputModules.end(); ++it)
	{
		(*it)->keyPressed(arg);
	}

	return false;
}

bool Mob::keyReleased( const OIS::KeyEvent &arg )
{
	for(auto it = m_InputModules.begin(); it != m_InputModules.end(); ++it)
	{
		(*it)->keyReleased(arg);
	}

	return false;
}

bool Mob::mouseMoved( const OIS::MouseEvent &arg )
{
	for(auto it = m_InputModules.begin(); it != m_InputModules.end(); ++it)
	{
		(*it)->mouseMoved(arg);
	}

	return false;
}

bool Mob::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	for(auto it = m_InputModules.begin(); it != m_InputModules.end(); ++it)
	{
		(*it)->mousePressed(arg, id);
	}

	return false;
}

bool Mob::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	for(auto it = m_InputModules.begin(); it != m_InputModules.end(); ++it)
	{
		(*it)->mouseReleased(arg, id);
	}

	return false;
}

Mob::MobType Mob::GetMobType()
{
	return m_MyMobType;
}

bool Mob::IsOnGround()
{
	return m_IsOnGround;
}

Ogre::Vector3 Mob::GetCameraModelOffset()
{
	return m_CameraModelOffset;
}

bool Mob::UpdateOnGround()
{
	if(m_pRigidBody)
	{
		btTransform worldTransform = m_pRigidBody->getWorldTransform();
		btVector3 startPos = worldTransform.getOrigin();
		btVector3 castDir = m_pRigidBody->getGravity().normalized() * 0.5f;
		btCollisionWorld::ClosestRayResultCallback closestHitRayCallback(startPos, startPos + castDir);
		closestHitRayCallback.m_collisionFilterGroup = COLLISION_BUILDRAYCAST;		//todo: this needs its own collision define, but this one works for now
		closestHitRayCallback.m_collisionFilterMask = COLLISION_STRUCTURE | COLLISION_OBJ | COLLISION_MOB;
		
		btDiscreteDynamicsWorld& bulletWorld = Application::StaticGetDynamicsWorld();
		bulletWorld.rayTest(startPos, startPos + castDir, closestHitRayCallback);
		Atom* pHitAtom = NULL;

		if(closestHitRayCallback.hasHit())
		{
			m_IsOnGround = true;
		}
		else
		{
			m_IsOnGround = false;
			m_pRigidBody->activate(true);
		}
	}
	return m_IsOnGround;
}

int Mob::GetIntent()
{
	return m_Intent;
}
