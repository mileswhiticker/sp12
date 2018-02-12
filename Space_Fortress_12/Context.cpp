#include "Context.hpp"
#include "Mob.hpp"

#include <OGRE\OgreSceneNode.h>
//
#include <BulletCollision\CollisionShapes\btBoxShape.h>
#include <BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>
#include <LinearMath\btDefaultMotionState.h>
//
#include <OGRE\OgreSceneNode.h>
#include <OGRE\OgreEntity.h>

#include "CollisionDefines.h"
#include "BtOgreHelper.hpp"
#include "BulletHelper.hpp"

Context::Context(int a_FaceDir, int a_Quadrant, Atom* a_pOwnedAtom)
:	m_MyContextType(CONTEXT_UNKNOWN)
,	m_pOwnedAtom(a_pOwnedAtom)
,	m_pRigidBody(0)
,	m_pCollisionShape(0)
,	m_ContextAtomType(0)
,	m_ContextAtomSubType(0)
,	m_pTargetEntity(0)
,	m_pSceneNode(0)
,	m_Enabled(false)
{
	//create interaction physbody

	m_pCollisionShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
	btDefaultMotionState* groundMotionState = new btDefaultMotionState( btTransform(btQuaternion(0,0,0,1), OGRE2BT(a_pOwnedAtom->m_pAtomRootSceneNode->getPosition()) ) );
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, m_pCollisionShape, btVector3(0,0,0));
	m_pRigidBody = new btRigidBody(groundRigidBodyCI);
	m_pRigidBody->setUserPointer(this);
	
	//default to build - should this be a constructor argument? 
	//do we need context types still, or can we abstract it into virtual methods?
	m_MyContextType = Context::BUILD;
	btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();
	dynamicsWorld.addRigidBody(m_pRigidBody, CONTEXT_INTERACT, RAYCAST_INTERACT);
}

Context::ContextType Context::GetContextType()
{
	return m_MyContextType;
}

int Context::GetAtomType()
{
	return m_ContextAtomType;
}

int Context::GetAtomSubType()
{
	return m_ContextAtomSubType;
}

bool Context::AttemptInteract(Mob* a_pSourceMob, Atom* a_pUsedAtom, int a_InteractType)
{
	std::cout << "DEBUG: Context::AttemptInteract()" << std::endl;
	if(m_pOwnedAtom)
	{
		return m_pOwnedAtom->Interact(a_pSourceMob, this, a_InteractType, a_pUsedAtom);
	}

	return false;
}

void Context::SetEnabled(bool a_Enabled)
{
	if(m_pRigidBody)
	{
		if(m_Enabled)
		{
			GetDynamicsWorld().removeRigidBody(m_pRigidBody);
			m_Enabled = false;
		}
		else
		{
			GetDynamicsWorld().addRigidBody(m_pRigidBody);
			m_Enabled = true;
		}
	}
}
