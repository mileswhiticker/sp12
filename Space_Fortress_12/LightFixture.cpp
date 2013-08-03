#include "LightFixture.hpp"

#include <OGRE\OgreEntity.h>
#include <OGRE/OgreSceneNode.h>

#include <LinearMath\btVector3.h>
#include <BulletCollision\CollisionShapes\btBoxShape.h>
#include <LinearMath\btDefaultMotionState.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>
#include <BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h>

#include "BulletHelper.hpp"
#include "BtOgreHelper.hpp"
#include "num2string.h"
#include "OgreHelper.hpp"
#include "MapHelper.hpp"

#include "CollisionDefines.h"
#include "Direction.h"

LightFixture::LightFixture(Turf* m_pLocTurf, int a_Dir)
:	Structure(m_pLocTurf, a_Dir)
{
	m_MyStructureType = Structure::LIGHTFIXTURE;
	m_MaterialName = "light_fixture";
	m_SelectMaterialName = "light_fixture_modulate";
	m_VirtualSelectMaterialName = "light_fixture_modulate";
}

void LightFixture::InstantiateStructure(bool a_IsBuildPoint)
{
	//an overlay plate is essentially an "outer cover" for the tile in one of the six cardinal directions
	//system is currently setup to handle any combination of the six, but it probably shouldn't be
	m_IsBuildPoint = a_IsBuildPoint;
	Ogre::SceneManager& sceneManager = GetSceneManager();
	//std::cout << "instantiating OverlayPlating with direction " << m_Direction << std::endl;
	
	//create entity
	m_pAtomEntity = sceneManager.createEntity("LightTubeFixture_" + num2string(m_AtomID), "obj_light_fixture_tube.mesh");
	m_pAtomRootSceneNode->attachObject(m_pAtomEntity);
	StopFlashingColour();

	//set up the directional offsets
	Ogre::Vector3 offsetPos(0, 0, 0);
	/*if(m_Direction & NORTH)
	{
		offsetPos.z += 0.395f;
	}
	if(m_Direction & SOUTH)
	{
		offsetPos.z -= 0.395f;
	}
	if(m_Direction & EAST)
	{
		offsetPos.x += 0.395f;
	}
	if(m_Direction & WEST)
	{
		offsetPos.x -= 0.395f;
	}
	if(m_Direction & UP)
	{
		offsetPos.y += 0.395f;
	}
	if(m_Direction & DOWN)
	{
		offsetPos.y -= 0.395f;
	}
	m_pAtomRootSceneNode->setPosition(offsetPos);
	m_pAtomRootSceneNode->lookAt(GetUnitVectorFromDir(m_Direction), Ogre::Node::TS_LOCAL);*/
	//m_pAtomRootSceneNode->yaw(Ogre::Degree(90));
	
	//create physics body and initialise to starting position
	m_pCollisionShape = new btBoxShape(btVector3(0.3f,0.3f,0.3f));
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
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_BUILDPOINT, RAYCAST_BUILD);

		//todo: is this working?
		m_pRigidBody->setCollisionFlags(m_pRigidBody->CF_NO_CONTACT_RESPONSE);
	}
	else
	{
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_STRUCTURE, RAYCAST_BUILD);
	}
}

void LightFixture::CreateFromBuildPoint()
{
	/*if(m_IsBuildPoint)
	{
		//first, reset the collision flags for build raycasting
		btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();
		dynamicsWorld.removeRigidBody(m_pRigidBody);
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_STRUCTURE, RAYCAST_BUILD);
		//m_pRigidBody->setCollisionFlags(m_pRigidBody->CF_STATIC_OBJECT);
		
		//reset the material
		m_pAtomEntity->setMaterialName("over_plating");

		//done
		SetEntityVisible(true);
		m_IsBuildPoint = false;
	}*/
}

void LightFixture::DestroyToBuildPoint()
{
	/*if(!m_IsBuildPoint)
	{
		//reset the rigidbody's collision flags for build raycasting
		btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();
		dynamicsWorld.removeRigidBody(m_pRigidBody);
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_BUILDPOINT, RAYCAST_BUILD);
		
		//reset the material
		m_pAtomEntity->setMaterialName("cell_highlight_material");

		//done
		SetEntityVisible(false);
		m_IsBuildPoint = true;
	}*/
}
