#include "Human.hpp"

#include <OGRE\OgreSceneManager.h>
#include <OGRE\OgreSceneNode.h>
#include <OGRE\OgreEntity.h>
#include <OGRE\OgreSubEntity.h>

#include <BulletCollision\CollisionShapes\btCapsuleShape.h>
#include <BulletCollision\CollisionShapes\btBoxShape.h>
#include <BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>
#include <LinearMath\btDefaultMotionState.h>

#include "InputModule_Generic.hpp"
#include "InputModule_MobLook.hpp"
#include "InputModule_MobWalk.hpp"
#include "InputModule_GhostMob.h"
//#include "AtomManager.hpp"
//#include "MapSuite.hpp"

#include "BtOgreHelper.hpp"
#include "OgreHelper.hpp"
#include "BulletHelper.hpp"
#include "RandHelper.h"

#include "MapCell.hpp"
#include "num2string.h"
#include "UID.hpp"
#include "Direction.h"
#include "DebugDrawer.h"
#include "CollisionDefines.h"

Human::Human(Ogre::Vector3 a_StartPos, int a_Direction)
:	Mob(a_StartPos, a_Direction)
{
	m_MyMobType = HUMAN;
	SetupInputModules();
	m_CameraModelOffset.y = 0.35f;
}

void Human::Update(float a_DeltaT)
{
	Mob::Update(a_DeltaT);
}

void Human::InstantiateAtom()
{
	//a single cuboid girder to cover this cell
	Ogre::SceneManager& sceneManager = GetSceneManager();
	m_pAtomEntity = sceneManager.createEntity("human_" + num2string(NewUID()), "ninja.mesh");
	m_pAtomEntitySceneNode->attachObject(m_pAtomEntity);
	m_pAtomEntitySceneNode->scale(0.004f, 0.004f, 0.004f);
	m_pAtomEntitySceneNode->setPosition(0, -0.4f, 0);
	
	//create physics collider
	btVector3 halfExtents = btVector3(0.15f, 0.4f, 0.1f);
	m_pCollisionShape = new btBoxShape(halfExtents);
	btDefaultMotionState* startMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), OGRE2BT(m_pAtomEntitySceneNode->_getDerivedPosition())));
	btScalar mass = 100.f;
	btVector3 fallInertia(0,0,0);
	m_pCollisionShape->calculateLocalInertia(mass,fallInertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, startMotionState, m_pCollisionShape, fallInertia);
	m_pRigidBody = new btRigidBody(rigidBodyCI);
	m_pRigidBody->setUserPointer(this);
	
	InitCollisionShapeDebugDraw(Ogre::ColourValue::Green);

	//todo: is this working?
	//m_pRigidBody->setCollisionFlags(m_pRigidBody->CF_NO_CONTACT_RESPONSE);

	//add new rigid body to world
	btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();
	dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_MOB, COLLISION_STRUCTURE | COLLISION_MOB);
	
	Mob::InstantiateAtom();
}

void Human::SetupInputModules()
{
	m_InputModules.push_back(new Generic(this, NULL));
	m_InputModules.push_back(new MobLook(this, NULL));
	m_InputModules.push_back(new MobWalk(this, NULL));
	m_InputModules.push_back(new GhostMob(this, NULL));
}

