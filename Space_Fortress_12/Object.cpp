#include "Object.hpp"

#include "BtOgreHelper.hpp"
#include "BulletHelper.hpp"
#include "OgreHelper.hpp"
#include "num2string.h"
#include "DebugDrawer.h"
#include "CollisionDefines.h"

#include "Mob.hpp"
#include "Cached.hpp"
#include "EffectManager.hpp"
#include "RandHelper.h"

#include <BulletCollision\CollisionShapes\btBoxShape.h>
#include <BulletCollision\CollisionShapes\btSphereShape.h>
#include <BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>
#include <LinearMath\btDefaultMotionState.h>

#include <OGRE\OgreSceneManager.h>
#include <OGRE\OgreSceneNode.h>
#include <OGRE\OgreEntity.h>

Object::Object(Ogre::Vector3 a_StartPos, int a_StartDirection)
:	Atom(a_StartPos, a_StartDirection)
,	m_MyObjType(UNKNOWN)
,	m_pHoldingMob(NULL)
{
	//
}

Box::Box(Ogre::Vector3 a_Position)
:	Object(a_Position, 0)
{
	//scenenode
	m_MyObjType = Object::BOX;
	m_MeshName = "cell_filling.mesh";
}

void Object::Update(float a_DeltaT)
{
	Atom::Update(a_DeltaT);

	/*if(DebugDrawer::getSingleton().getEnabled() && m_pRigidBody && m_pCollisionShape)
	{
		btTransform& transform = m_pRigidBody->getWorldTransform();
		Ogre::Vector3 pVertices[8];
		btBoxShape* pBoxShape = (btBoxShape*)m_pCollisionShape;
		for(int j = 0; j < 8; j++)
		{
			btVector3 vert;
			pBoxShape->getVertex(j, vert);

			//this is the vert position without any rotation
			pVertices[7 - j] = BT2OGRE(vert + transform.getOrigin());
		}
		
		DebugDrawer::getSingleton().drawCuboid(pVertices, Ogre::ColourValue::Red, true);
	}*/
}

void Object::InstantiateAtom()
{
	Ogre::SceneManager& sceneManager = GetSceneManager();
	m_pAtomEntity = sceneManager.createEntity("obj_" + num2string(m_AtomID), m_MeshName);
	m_pAtomEntitySceneNode->attachObject(m_pAtomEntity);
	m_pAtomEntitySceneNode->setScale(0.25f, 0.25f, 0.25f);

	//m_pAtomEntity->setMaterialName("madmarxOutLine");

	//create physics body and initialise to starting position
	btVector3 halfExtents(0.125f, 0.125f, 0.125f);
	m_pCollisionShape = new btBoxShape(halfExtents);
	//m_pCollisionShape = new btSphereShape(0.5);
	btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), OGRE2BT(m_pAtomEntitySceneNode->_getDerivedPosition())));
	btScalar mass = 1;
	btVector3 fallInertia(0,0,0);
	m_pCollisionShape->calculateLocalInertia(mass,fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, m_pCollisionShape, fallInertia);
	m_pRigidBody = new btRigidBody(fallRigidBodyCI);
	m_pRigidBody->setSleepingThresholds(0,0);

	//add new rigid body to world
	btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();
	dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_OBJ, COLLISION_OBJ|COLLISION_MOB|COLLISION_STRUCTURE);
	
	InitCollisionShapeDebugDraw(Ogre::ColourValue::Blue);
	
	if(m_pRigidBody)
	{
		m_pRigidBody->applyForce(btVector3(fRand(-2.f,2.f), fRand(-2.f,2.f), fRand(-2.f,2.f)), btVector3(fRand(-0.1f,0.1f), fRand(-0.1f,0.1f), fRand(-0.1f,0.1f)));
	}
}

Object::ObjType Object::GetObjType()
{
	return m_MyObjType;
}

void Object::Build(Turf* a_pLocTurf, int a_Direction, int a_Layer)
{
	//
}

void Object::AddToMobInventory(Mob* a_pMob)
{
	if( a_pMob && (a_pMob != m_pHoldingMob) )
	{
		//if we're already being held, tell them to forget about us
		if(m_pHoldingMob)
		{
			//this way avoids an endless recursive loop
			Mob* pHoldingMob = m_pHoldingMob;
			m_pHoldingMob = NULL;
			pHoldingMob->RemoveObjectFromInventory(this);
		}
		else
		{
			//make the mesh invisible
			m_pAtomEntitySceneNode->detachObject(m_pAtomEntity);
		
			//disable the rigidbody
			btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();
			dynamicsWorld.removeRigidBody(m_pRigidBody);
			
			m_UseRigidbodyPosition = false;
		}
		
		m_pHoldingMob = a_pMob;
		a_pMob->AddObjectToInventory(this);
		
		//bind the object position to the mob position
		if(m_pAtomRootSceneNode->getParentSceneNode())
		{
			m_pAtomRootSceneNode->getParentSceneNode()->removeChild(m_pAtomRootSceneNode);
		}
		m_pAtomRootSceneNode->setPosition(0,0,0);
		m_pHoldingMob->m_pAtomRootSceneNode->addChild(m_pAtomRootSceneNode);
	}
}

void Object::RemoveFromMobInventory()
{
	if(m_pHoldingMob)
	{
		m_pHoldingMob = NULL;
		m_pHoldingMob->RemoveObjectFromInventory(this);

		//make the mesh visible
		Ogre::SceneNode* pCurParent = m_pAtomEntity->getParentSceneNode();
		if(pCurParent != m_pAtomEntitySceneNode)
		{
			if(pCurParent)
			{
				pCurParent->detachObject(m_pAtomEntity);
			}
			m_pAtomEntitySceneNode->attachObject(m_pAtomEntity);
		}

		//detach our scene node from the mob, and child it to the root scene node
		Ogre::SceneNode& rootSceneNode = GetRootSceneNode();
		Ogre::SceneNode* pParentSceneNode = m_pAtomRootSceneNode->getParentSceneNode();
		if(pParentSceneNode != &rootSceneNode)
		{
			if(pParentSceneNode)
			{
				pParentSceneNode->removeChild(m_pAtomRootSceneNode);
			}
			rootSceneNode.addChild(m_pAtomRootSceneNode);
		}
		
		//re-enable the rigidbody
		btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_OBJ, COLLISION_OBJ|COLLISION_MOB|COLLISION_STRUCTURE);
		m_UseRigidbodyPosition = true;
	}
}
