#include "OverlayPlating.hpp"

#include <OGRE\OgreSceneManager.h>
#include <OGRE\OgreSceneNode.h>
#include <OGRE\OgreEntity.h>
#include <OGRE\OgreSubEntity.h>

#include <BulletCollision\CollisionShapes\btBoxShape.h>
#include <BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>
#include <LinearMath\btDefaultMotionState.h>
#include <LinearMath\btVector3.h>

#include "BtOgreHelper.hpp"
#include "OgreHelper.hpp"
#include "BulletHelper.hpp"

#include "CollisionDefines.h"
#include "Direction.h"
#include "num2string.h"
#include "UID.hpp"

OverlayPlating::OverlayPlating(Ogre::Vector3 a_Pos, int a_Dir)
:	Structure(a_Pos, a_Dir)
{
	m_MyStructureType = Structure::OVERLAYPLATING;
}

void OverlayPlating::InstantiateStructure(bool a_IsBuildPoint)
{
	//an overlay plate is essentially an "outer cover" for the tile in one of the six cardinal directions
	//system is currently setup to handle any combination of the six, but it probably shouldn't be
	m_IsBuildPoint = a_IsBuildPoint;
	Ogre::SceneManager& sceneManager = GetSceneManager();
	//std::cout << "instantiating OverlayPlating with direction " << m_Direction << std::endl;
	
	//create entity
	m_pAtomEntity = sceneManager.createEntity("overlayplating_" + num2string(NewUID()), "cell_overlay.mesh");
	m_pAtomEntitySceneNode->attachObject(m_pAtomEntity);
	StopFlashingColour();

	//set up the directional offsets
	Ogre::Vector3 offsetPos(0, 0, 0);
	Ogre::Vector3 lookatPos(0, 0, 0);
	btVector3 halfExtents(0.5f, 0.5f, 0.5f);
	//std::cout << "	new overlay plating" << std::endl;
	if(m_Direction & NORTH)
	{
		offsetPos.z += 0.505f;
		lookatPos.z += 1;
		halfExtents.setZ(0.005f);
		//std::cout << "NORTH " << (isPhysical ? "plating" : "trigger") << std::endl;
	}
	if(m_Direction & SOUTH)
	{
		offsetPos.z -= 0.505f;
		lookatPos.z -= 1;
		halfExtents.setZ(0.005f);
		//std::cout << "SOUTH " << (isPhysical ? "plating" : "trigger") << std::endl;
	}
	if(m_Direction & EAST)
	{
		offsetPos.x += 0.505f;
		lookatPos.x += 1;
		halfExtents.setX(0.005f);
		//std::cout << "EAST " << (isPhysical ? "plating" : "trigger") << std::endl;
	}
	if(m_Direction & WEST)
	{
		offsetPos.x -= 0.505f;
		lookatPos.x -= 1;
		halfExtents.setX(0.005f);
		//std::cout << "WEST " << (isPhysical ? "plating" : "trigger") << std::endl;
	}
	if(m_Direction & UP)
	{
		offsetPos.y += 0.505f;
		lookatPos.y += 1;
		halfExtents.setY(0.005f);
		//std::cout << "UP " << (isPhysical ? "plating" : "trigger") << std::endl;
	}
	if(m_Direction & DOWN)
	{
		offsetPos.y -= 0.505f;
		lookatPos.y -= 1;
		halfExtents.setY(0.005f);
		//std::cout << "DOWN " << (isPhysical ? "plating" : "trigger") << std::endl;
	}
	m_pAtomEntitySceneNode->setPosition(offsetPos);
	m_pAtomEntitySceneNode->lookAt(lookatPos, Ogre::Node::TS_LOCAL);
	m_pAtomEntitySceneNode->yaw(Ogre::Degree(90));
	
	//create physics body and initialise to starting position
	m_pCollisionShape = new btBoxShape(halfExtents);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), OGRE2BT(m_pAtomEntitySceneNode->_getDerivedPosition())));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, m_pCollisionShape, btVector3(0,0,0));
	m_pRigidBody = new btRigidBody(groundRigidBodyCI);
	m_pRigidBody->setUserPointer(this);

	//add new rigid body to world
	btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();
	if(m_IsBuildPoint)
	{
		SetEntityVisible(false);
		m_pAtomEntity->setMaterialName("cell_highlight_material");
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_BUILDPOINT, COLLISION_BUILDRAYCAST);

		//todo: is this working?
		m_pRigidBody->setCollisionFlags(m_pRigidBody->CF_NO_CONTACT_RESPONSE);
	}
	else
	{
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_STRUCTURE, COLLISION_BUILDRAYCAST);
	}
}

void OverlayPlating::CreateFromBuildPoint()
{
	if(m_IsBuildPoint)
	{
		//first, reset the collision flags for build raycasting
		btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();
		dynamicsWorld.removeRigidBody(m_pRigidBody);
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_STRUCTURE, COLLISION_BUILDRAYCAST);
		//m_pRigidBody->setCollisionFlags(m_pRigidBody->CF_STATIC_OBJECT);
		
		//reset the material
		m_pAtomEntity->setMaterialName("over_plating");

		//done
		SetEntityVisible(true);
		m_IsBuildPoint = false;
	}
}

void OverlayPlating::DestroyToBuildPoint()
{
	if(!m_IsBuildPoint)
	{
		//reset the rigidbody's collision flags for build raycasting
		btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();
		dynamicsWorld.removeRigidBody(m_pRigidBody);
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_BUILDPOINT, COLLISION_BUILDRAYCAST);
		
		//reset the material
		m_pAtomEntity->setMaterialName("cell_highlight_material");

		//done
		SetEntityVisible(false);
		m_IsBuildPoint = true;
	}
}

void OverlayPlating::Select(ObserverBuild* a_pSelectingObserver)
{
	Atom::Select(a_pSelectingObserver);
	if(m_pAtomEntity)
	{
		m_pAtomEntity->setMaterialName("over_plating_modulate");
	}
}

void OverlayPlating::DeSelect(ObserverBuild* a_pSelectingObserver)
{
	Atom::DeSelect(a_pSelectingObserver);
	if(m_pAtomEntity)
	{
		m_pAtomEntity->setMaterialName("over_plating");
	}
}
