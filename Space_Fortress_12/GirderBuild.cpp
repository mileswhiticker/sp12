#include "GirderBuild.hpp"
#include "Turf.hpp"
#include "Mob.hpp"

//#include <BulletCollision\CollisionShapes\btBox2dShape.h>

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

GirderBuild::GirderBuild(Turf* a_pLocTurf, Atom* a_pOwnedAtom)
:	Context(a_pLocTurf, a_pOwnedAtom)
{
	m_ContextAtomType = Atom::TURF;
	m_ContextAtomSubType = Turf::GIRDER;

	if(a_pLocTurf)
	{
		//create physics collider to intercept raycasts
		m_pCollisionShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
		btDefaultMotionState* groundMotionState = new btDefaultMotionState( btTransform(btQuaternion(0,0,0,1), OGRE2BT(a_pLocTurf->m_pAtomRootSceneNode->getPosition()) ) );
		btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, m_pCollisionShape, btVector3(0,0,0));
		m_pRigidBody = new btRigidBody(groundRigidBodyCI);
		m_pRigidBody->setUserPointer(this);
	
		bool valid = true;
		switch(a_pLocTurf->GetTurfType())
		{
		case(Turf::EMPTY):
			{
				m_MyContextType = Context::BUILD;
				break;
			}
		case(Turf::GIRDER):
			{
				m_MyContextType = Context::UNBUILD;
				break;
			}
		default:
			{
				valid = false;
				break;
			}
		}

		if(valid)
		{
			btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();
			dynamicsWorld.addRigidBody(m_pRigidBody, CONTEXT_INTERACT, RAYCAST_INTERACT);
		}
	}
}

bool GirderBuild::AttemptInteract(Mob* a_pSourceMob, Atom* a_pUsedAtom, int a_InteractType)
{
	std::cout << "DEBUG: GirderBuild::AttemptInteract()" << std::endl;

	switch(a_InteractType)
	{
	case(Mob::TARGET):
		{
			if(m_pTargetEntity && m_pSceneNode && !m_pTargetEntity->getParentSceneNode())
			{
				m_pSceneNode->attachObject(m_pTargetEntity);
			}
			break;
		}
	case(Mob::UNTARGET):
		{
			if(m_pTargetEntity && m_pSceneNode && m_pTargetEntity->getParentSceneNode())
			{
				m_pSceneNode->detachObject(m_pTargetEntity);
			}
			break;
		}
	case(Mob::INTERACT):
		{
			Ogre::SceneNode* pSceneNode = m_pTargetEntity->getParentSceneNode();
			if(pSceneNode)
			{
				pSceneNode->detachObject(m_pTargetEntity);
			}
			break;
		}
	default:
		{
			return Context::AttemptInteract(a_pSourceMob, a_pUsedAtom, a_InteractType);
		}
	}

	return true;
}
