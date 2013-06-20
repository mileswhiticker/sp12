#include "Object.hpp"

#include "BtOgreHelper.hpp"
#include "BulletHelper.hpp"
#include "OgreHelper.hpp"
#include "UID.hpp"
#include "num2string.h"
#include "DebugDrawer.h"
#include "CollisionDefines.h"

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
{
	//
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
	if(m_pRigidBody)
	{
		m_pRigidBody->applyForce(btVector3(fRand(-2,2), fRand(-2,2), fRand(-2,2)), btVector3(fRand(-0.1f,0.1f), fRand(-0.1f,0.1f), fRand(-0.1f,0.1f)));
	}
}

Object::ObjType Object::GetObjType()
{
	return m_MyObjType;
}

Box::Box(Ogre::Vector3 a_Position)
:	Object(a_Position, 0)
{
	//scenenode
	m_MyObjType = Object::BOX;
}

void Box::InstantiateAtom()
{
	Ogre::SceneManager& sceneManager = GetSceneManager();
	m_pAtomEntity = sceneManager.createEntity("box_" + num2string(NewUID()), "cell_filling.mesh");
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
	
	Object::InstantiateAtom();
}
