#include "Object.hpp"
#include "Mob.hpp"
#include "InputModule.hpp"

#include "BtOgreHelper.hpp"
#include "BulletHelper.hpp"
#include "OgreHelper.hpp"
#include "num2string.h"
#include "DebugDrawer.h"
#include "CollisionDefines.h"

#include "Cached.hpp"
#include "EffectManager.hpp"

#include <BulletCollision\CollisionShapes\btBoxShape.h>
#include <BulletCollision\CollisionShapes\btSphereShape.h>
#include <BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>
#include <LinearMath\btDefaultMotionState.h>

#include <OGRE\OgreSceneManager.h>
#include <OGRE\OgreSceneNode.h>
#include <OGRE\OgreEntity.h>

#include "num2string.h"
#include "RandHelper.h"

Object::Object(Ogre::Vector3 a_StartPos, int a_StartDirection)
:	Atom(a_StartPos, a_StartDirection)
,	m_MyObjType(OBJTYPE_UNKNOWN)
,	m_pHoldingMob(NULL)
,	m_pEquipInputModule(NULL)
{
	m_AtomTextName = "Object";
	m_DefaultPhysicsGroup = COLLISION_OBJ;
	m_DefaultPhysicsMask = COLLISION_OBJ|COLLISION_MOB|COLLISION_STRUCTURE;
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

InputModule* Object::GetEquipInputModule()
{
	return m_pEquipInputModule;
}

bool Object::AddToOtherAtomContents(Atom* a_pAtom)
{
	if(Atom::AddToOtherAtomContents(a_pAtom))
	{
		if(m_pEquipInputModule && m_pHoldingAtom->GetAtomType() == Atom::MOB)
		{
			Mob* pHoldingMob = (Mob*)a_pAtom;
			m_pEquipInputModule->SetMobOwner(pHoldingMob);
			//pHoldingMob-
		}
		return true;
	}
	return false;
}

bool Object::RemoveFromOtherAtomContents()
{
	if(Atom::RemoveFromOtherAtomContents())
	{
		//we should only have a mob owner if we're directly in the contents of the mob
		if(m_pEquipInputModule)
		{
			m_pEquipInputModule->SetMobOwner(NULL);
		}
		return true;
	}
	return false;
}
