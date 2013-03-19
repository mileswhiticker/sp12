#include "Object.hpp"

#include "Application.hpp"

#include "BtOgreHelper.hpp"
#include "OgreHelper.hpp"
#include "UID.hpp"
#include "num2string.h"
#include "DebugDrawer.h"

#include <BulletCollision\CollisionShapes\btBoxShape.h>
#include <BulletCollision\CollisionShapes\btSphereShape.h>
#include <BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>
#include <LinearMath\btDefaultMotionState.h>

#include <OGRE\OgreSceneManager.h>
#include <OGRE\OgreSceneNode.h>
#include <OGRE\OgreEntity.h>

Object::Object()
:	Atom()
{
	//
}

void Object::Update(float a_DeltaT)
{
	Atom::Update(a_DeltaT);

	if(DebugDrawer::getSingleton().getEnabled() && m_pRigidBody && m_pCollisionShape)
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
	}
}

Box::Box(Ogre::Vector3 a_Position)
:	Object()
{
	//scenenode
	Ogre::SceneNode& rootSceneNode = GetRootSceneNode();
	m_pAtomSceneNode = rootSceneNode.createChildSceneNode();
	m_pAtomSceneNode->setPosition(a_Position);
	
	Instantiate();
}

void Box::Instantiate()
{
	Ogre::SceneManager& sceneManager = GetSceneManager();
	m_pAtomEntity = sceneManager.createEntity(num2string(NewUID()) + " obj", "cell_filling.mesh");
	m_pAtomSceneNode->attachObject(m_pAtomEntity);
	m_pAtomSceneNode->setScale(0.25f, 0.25f, 0.25f);

	//m_pAtomEntity->setMaterialName("madmarxOutLine");

	//create physics body and initialise to starting position
	m_pCollisionShape = new btBoxShape( btVector3(0.125f, 0.125f, 0.125f) );
	//m_pCollisionShape = new btSphereShape(0.5);
	btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), OGRE2BT(m_pAtomSceneNode->getPosition())));
	btScalar mass = 1;
	btVector3 fallInertia(0,0,0);
	m_pCollisionShape->calculateLocalInertia(mass,fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, m_pCollisionShape, fallInertia);
	m_pRigidBody = new btRigidBody(fallRigidBodyCI);

	//add new rigid body to world
	btDiscreteDynamicsWorld& dynamicsWorld = Application::StaticGetDynamicsWorld();
	dynamicsWorld.addRigidBody(m_pRigidBody);
}
