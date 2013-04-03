#include "Girder.hpp"

#include <OGRE\OgreSceneManager.h>
#include <OGRE\OgreSceneNode.h>
#include <OGRE\OgreEntity.h>
#include <OGRE\OgreSubEntity.h>

#include <BulletCollision\CollisionShapes\btBoxShape.h>
#include <BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>
#include <LinearMath\btDefaultMotionState.h>

#include "AtomManager.hpp"
#include "MapSuite.hpp"

#include "BtOgreHelper.hpp"
#include "OgreHelper.hpp"
#include "BulletHelper.hpp"

#include "MapCell.hpp"
#include "num2string.h"
#include "UID.hpp"
#include "Direction.h"
#include "DebugDrawer.h"
#include "CollisionDefines.h"

Girder::Girder(MapCell* a_pSourceMapCell)
:	Structure()
,	Turf()
,	m_PlateOverlayDirs(0)
,	m_PlateUnderlays(0)
,	m_pSourceMapCell(a_pSourceMapCell)
{
	m_pMyStructure = this;
	m_MyAtomType = Atom::STRUCTURE;
	m_MyStructureType = Structure::GIRDER;
	//
	m_pAtomSceneNode = NewSceneNode();
	if(m_pSourceMapCell)
	{
		m_pAtomSceneNode->setPosition(m_pSourceMapCell->m_Position);
		a_pSourceMapCell->m_pMyCellTurf = this;
	}
}

void Girder::InstantiateStructure(bool a_IsBuildPoint)
{
	m_IsBuildPoint = a_IsBuildPoint;

	//a single cuboid girder to cover this cell
	Ogre::SceneManager& sceneManager = GetSceneManager();
	m_pAtomEntity = sceneManager.createEntity(num2string(NewUID()) + " girder", "girder.mesh");
	m_pAtomSceneNode->attachObject(m_pAtomEntity);
	StopFlashingColour();
	
	//create physics collider to intercept raycasts
	btBoxShape* m_pCollisionShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), OGRE2BT(m_pAtomSceneNode->getPosition())));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, m_pCollisionShape, btVector3(0,0,0));
	m_pRigidBody = new btRigidBody(groundRigidBodyCI);
	m_pRigidBody->setUserPointer(this);
	
	//todo: is this working?
	m_pRigidBody->setCollisionFlags(m_pRigidBody->CF_NO_CONTACT_RESPONSE);

	//add new rigid body to world
	btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();
	if(m_IsBuildPoint)
	{
		//std::cout << "Instantiate() called on structure build point" << std::endl;
		SetEntityVisible(false);
		m_pAtomEntity->setMaterialName("cell_highlight_material");
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_BUILDPOINT, COLLISION_BUILDRAYCAST);
	}
	else
	{
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_STRUCTURE, COLLISION_BUILDRAYCAST);

		//create overlay buildpoints
		for(int curDir = 1; curDir <= 32; curDir *= 2)
		{
			//std::cout << "direction: " << curDir << std::endl;
			Structure* pUnusedBuildPoint = AtomManager::GetSingleton().CreateStructure(Structure::OVERLAYPLATING, m_pSourceMapCell, NULL, curDir|BUILD_POINT|INSTANTIATE_IMMEDIATELY);
			m_UnusedBuildPoints.push_back(pUnusedBuildPoint);
		}
	}
}

void Girder::AddFreefloatingObj(std::string a_TypeTag)
{
	if(m_pAtomSceneNode)
	{
		AtomManager::GetSingleton().CreateAtom(Atom::OBJECT, m_pAtomSceneNode->getPosition());
	}
	else
	{
		std::cout << "WARNING: Trying to add object of type '" << a_TypeTag << "' to empty (uninitialised?) cell." << std::endl;
	}
}

void Girder::CreateFromBuildPoint()
{
	if(m_IsBuildPoint)
	{
		std::cout << "creating girder from build point" << std::endl;

		//update the collision flags
		btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();
		dynamicsWorld.removeRigidBody(m_pRigidBody);
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_STRUCTURE, COLLISION_BUILDRAYCAST);

		//create overlay buildpoints
		Structure* pUnusedBuildPoint;
		for(int curDir = 1; curDir <= 32; curDir *= 2)
		{
			//std::cout << "direction: " << curDir << std::endl;
			pUnusedBuildPoint = AtomManager::GetSingleton().CreateStructure(Structure::OVERLAYPLATING, m_pSourceMapCell, NULL, INSTANTIATE_IMMEDIATELY|BUILD_POINT);
			m_UnusedBuildPoints.push_back(pUnusedBuildPoint);
			pUnusedBuildPoint->ChangeDirection(curDir);
			pUnusedBuildPoint->InstantiateStructure(true);
		}

		//miscellaneous
		m_pAtomEntity->setMaterialName("girder_material");
		SetEntityVisible(true);
		m_IsBuildPoint = false;
		
		//create girder buildpoints in adjacent cells
		Ogre::Vector3 pos = m_pAtomSceneNode->getPosition();
		MapSuite::GetInstance().CreateAdjacentGirderBuildpoints(m_pSourceMapCell);
	}
}

void Girder::DestroyToBuildPoint()
{
	if(!m_IsBuildPoint)
	{
		std::cout << "destroying girder to build point" << std::endl;

		//reset the collision flags for build raycasting
		btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();
		dynamicsWorld.removeRigidBody(m_pRigidBody);
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_BUILDPOINT, COLLISION_BUILDRAYCAST);

		//reset the material
		m_pAtomEntity->setMaterialName("cell_highlight_material");

		//done
		SetEntityVisible(false);
		m_IsBuildPoint = true;

		MapSuite::GetInstance().ClearDependantAdjacentGirderBuildpoints(m_pSourceMapCell);
	}
}

/*void Girder::ResetEmptyOverlays()
{
	for(int i=1; i<=32; i*=2)
	{
		if( !(m_PlateOverlayDirs&i) )
		{
			AddOverlay(i, "");
		}
	}
	//m_PlateOverlayDirs
}*/

/*bool Girder::AddOverlay(int a_Dir, std::string a_OverlayID)
{
	//an overlay plate is essentially an "outer cover" for the tile in one (of six) directions
	bool isPhysical = false;
	if(a_OverlayID.length())
		isPhysical = true;
	Ogre::SceneManager& sceneManager = GetSceneManager();
	Ogre::SceneNode* pOverlayNode = m_pAtomSceneNode->createChildSceneNode();

	//only create an entity if it's a valid id
	if(isPhysical)
	{
		m_Overlays.push_back(sceneManager.createEntity(num2string(NewUID()) + " overlay", "cell_overlay.mesh"));
		pOverlayNode->attachObject(m_Overlays.back());
	}

	Ogre::Vector3 offsetPos(0, 0, 0);
	Ogre::Vector3 lookatPos(0, 0, 0);
	btVector3 halfExtents(0.5f, 0.5f, 0.5f);
	//
	if(a_Dir & NORTH)
	{
		//offsetPos.z += 0.505f;
		lookatPos.z += 1;
		halfExtents.setZ(0.005f);
		//std::cout << "NORTH " << (isPhysical ? "plating" : "trigger") << std::endl;
	}
	if(a_Dir & SOUTH)
	{
		//offsetPos.z -= 0.505f;
		lookatPos.z -= 1;
		halfExtents.setZ(0.005f);
		//std::cout << "SOUTH " << (isPhysical ? "plating" : "trigger") << std::endl;
	}
	if(a_Dir & EAST)
	{
		offsetPos.x += 0.505f;
		lookatPos.x += 1;
		halfExtents.setX(0.005f);
		//std::cout << "EAST " << (isPhysical ? "plating" : "trigger") << std::endl;
	}
	if(a_Dir & WEST)
	{
		offsetPos.x -= 0.505f;
		lookatPos.x -= 1;
		halfExtents.setX(0.005f);
		//std::cout << "WEST " << (isPhysical ? "plating" : "trigger") << std::endl;
	}
	if(a_Dir & UP)
	{
		offsetPos.y += 0.505f;
		lookatPos.y += 1;
		halfExtents.setY(0.005f);
		//std::cout << "UP " << (isPhysical ? "plating" : "trigger") << std::endl;
	}
	if(a_Dir & DOWN)
	{
		offsetPos.y -= 0.505f;
		lookatPos.y -= 1;
		halfExtents.setY(0.005f);
		//std::cout << "DOWN " << (isPhysical ? "plating" : "trigger") << std::endl;
	}
	m_PlateOverlayDirs &= a_Dir;
	//
	pOverlayNode->lookAt(lookatPos, Ogre::Node::TS_LOCAL);
	pOverlayNode->setPosition(offsetPos);
	pOverlayNode->yaw(Ogre::Degree(90));

	//create physics body and initialise to starting position
	btBoxShape* pBoxShape = new btBoxShape(halfExtents);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), OGRE2BT(m_pAtomSceneNode->getPosition() + offsetPos)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, pBoxShape, btVector3(0,0,0));
	btRigidBody* pRigidBody = new btRigidBody(groundRigidBodyCI);
	
	//todo: is this working?
	if(!isPhysical)
		pRigidBody->setCollisionFlags(pRigidBody->CF_NO_CONTACT_RESPONSE);

	pRigidBody->setUserPointer(this);

	//add new rigid body to world
	btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();
	dynamicsWorld.addRigidBody(pRigidBody, COLLISION_STRUCTURE, COLLISION_BUILDRAYCAST);

	m_RigidBodies.push_back(pRigidBody);
	m_BoxCollisionShapes.push_back(pBoxShape);
	return true;
}*/

/*bool Girder::AddUnderlay(int a_Dir, std::string a_UnderlayID)
{
	//return false if one already exists, or the specified type doesn't exist
	if(!a_UnderlayID.compare("grey_default"))
	{
		//an overlay plate is essentially an "outer cover" for the tile in one (of six) directions
		Ogre::SceneManager& sceneManager = GetSceneManager();
		
		m_Underlays.push_back(sceneManager.createEntity(num2string(NewUID()) + " underlay", "cell_underlay.mesh"));
		Ogre::SceneNode* pUnderlayNode = m_pAtomSceneNode->createChildSceneNode();
		pUnderlayNode->attachObject(m_Underlays.back());
		std::string matName = m_Underlays.back()->getSubEntity(0)->getMaterialName();

		Ogre::Vector3 offsetPos(0, 0, 0);
		Ogre::Vector3 lookatPos(0, 0, 0);
		Ogre::Vector3 scale(0.9f, 0.9f, 0.9f);
		btVector3 halfExtents(0.5f, 0.5f, 0.5f);
		//
		if(a_Dir & NORTH)
		{
			offsetPos.z += 0.45f;
			lookatPos.z += 1;
			halfExtents.setZ(0.005f);
			//scale.z = 1;
		}
		if(a_Dir & SOUTH)
		{
			offsetPos.z -= 0.45f;
			lookatPos.z -= 1;
			halfExtents.setZ(0.005f);
			//scale.z = 1;
		}
		if(a_Dir & EAST)
		{
			offsetPos.x += 0.45f;
			lookatPos.x += 1;
			halfExtents.setX(0.005f);
			//scale.x = 1;
		}
		if(a_Dir & WEST)
		{
			offsetPos.x -= 0.45f;
			lookatPos.x -= 1;
			halfExtents.setX(0.005f);
			//scale.x = 1;
		}
		if(a_Dir & UP)
		{
			offsetPos.y += 0.45f;
			lookatPos.y += 1;
			halfExtents.setY(0.005f);
			//scale.y = 1;
		}
		if(a_Dir & DOWN)
		{
			offsetPos.y -= 0.45f;
			lookatPos.y -= 1;
			halfExtents.setY(0.005f);
			//scale.y = 1;
		}
		//
		pUnderlayNode->setPosition(offsetPos);
		pUnderlayNode->lookAt(lookatPos, Ogre::Node::TS_LOCAL);
		pUnderlayNode->setScale(scale);
		pUnderlayNode->yaw(Ogre::Degree(90));

		//create physics body and initialise to starting position
		btBoxShape* pBoxShape = new btBoxShape(halfExtents * OGRE2BT(scale));
		btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), OGRE2BT(m_pAtomSceneNode->getPosition() + offsetPos)));
		btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, pBoxShape, btVector3(0,0,0));
		btRigidBody* pRigidBody = new btRigidBody(groundRigidBodyCI);

		//add new rigid body to world
		btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();
		dynamicsWorld.addRigidBody(pRigidBody);

		m_RigidBodies.push_back(pRigidBody);
		m_BoxCollisionShapes.push_back(pBoxShape);

		return true;
	}
	return false;
}*/
