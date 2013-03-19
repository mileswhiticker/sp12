#include "BuildHighlight.hpp"

//#pragma warning(disable:4251)
#include "Application.hpp"
#include "num2string.h"
#include "UID.hpp"
#include "Direction.h"
#include "AtomManager.hpp"
#include "BtOgreHelper.hpp"
#include "DebugDrawer.h"

#include "CollisionDefines.h"
#include "OgreHelper.hpp"

#include <OGRE\OgreSceneManager.h>
#include <OGRE\OgreSceneNode.h>
#include <OGRE\OgreEntity.h>
#include <OGRE\OgreSubEntity.h>

#include <BulletCollision\CollisionShapes\btBoxShape.h>
#include <BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>
#include <LinearMath\btDefaultMotionState.h>

BuildHighlight::BuildHighlight(Ogre::Vector3 a_Pos)
:	Atom()
{
	m_MyAtomType = Atom::BUILDHIGHLIGHT;
	//
	m_pAtomSceneNode = NewSceneNode();
	m_pAtomSceneNode->setPosition(a_Pos);
	Instantiate();
}

void BuildHighlight::Instantiate()
{
	//a single cuboid girder to cover this cell
	Ogre::SceneManager& sceneManager = Application::StaticGetSceneManager();

	m_pAtomEntity = sceneManager.createEntity(num2string(NewUID()) + " girder", "girder.mesh");
	m_pAtomSceneNode->attachObject(m_pAtomEntity);
	StopFlashingColour();
	
	//create physics collider to intercept raycasts
	btBoxShape* pBoxShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), OGRE2BT(m_pAtomSceneNode->getPosition())));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, pBoxShape, btVector3(0,0,0));
	btRigidBody* pRigidBody = new btRigidBody(groundRigidBodyCI);
	
	//todo: is this working?
	pRigidBody->setCollisionFlags(pRigidBody->CF_NO_CONTACT_RESPONSE);

	pRigidBody->setUserPointer(this);

	//add new rigid body to world
	btDiscreteDynamicsWorld& dynamicsWorld = Application::StaticGetDynamicsWorld();
	dynamicsWorld.addRigidBody(pRigidBody, COLLISION_HIGHLIGHTCELL, COLLISION_BUILDRAYCAST);
}
