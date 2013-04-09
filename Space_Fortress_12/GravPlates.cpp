#include "GravPlates.hpp"

#include <OGRE\OgreSceneManager.h>
#include <OGRE\OgreSceneNode.h>
#include <OGRE\OgreEntity.h>
#include <OGRE\OgreSubEntity.h>

#include <BulletCollision\CollisionShapes\btBoxShape.h>
#include <BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>
#include <LinearMath\btDefaultMotionState.h>
#include <LinearMath\btVector3.h>

#include "OgreHelper.hpp"
#include "BulletHelper.hpp"
#include "BtOgreHelper.hpp"

#include "MapSuite.hpp"
#include "MapHelper.hpp"
#include "MapCell.hpp"
#include "CollisionDefines.h"
#include "Direction.h"
#include "num2string.h"
#include "UID.hpp"

GravPlates::GravPlates(MapCell* a_pMapCell, int a_Dir)
:	Structure(a_pMapCell, a_Dir)
,	m_CellRange(10)
,	m_InitialGravityForce(10)
,	m_LinearGravityFalloff(1)
,	m_GravityActive(false)
,	m_GravityReversed(false)
{
	m_MyStructureType = Structure::GRAVPLATES;
}

void GravPlates::InstantiateStructure(bool a_IsBuildPoint)
{
	//an overlay plate is essentially an "outer cover" for the tile in one of the six cardinal directions
	//system is currently setup to handle any combination of the six, but it probably shouldn't be
	m_IsBuildPoint = a_IsBuildPoint;
	Ogre::SceneManager& sceneManager = GetSceneManager();
	//std::cout << "instantiating OverlayPlating with direction " << m_Direction << std::endl;
	
	//create entity
	m_pAtomEntity = sceneManager.createEntity("gravplates_" + num2string(NewUID()), "gravplates.mesh");
	m_pAtomEntitySceneNode->attachObject(m_pAtomEntity);
	StopFlashingColour();

	//set up the directional offsets
	Ogre::Vector3 offsetPos(0, 0, 0);
	Ogre::Vector3 lookatPos(0, 0, 0);
	btVector3 halfExtents(0.5f, 0.5f, 0.5f);
	//std::cout << "	new overlay plating" << std::endl;
	if(m_Direction & NORTH)
	{
		offsetPos.z += 0.45;
		lookatPos.z += 1;
		halfExtents.setZ(0.005f);
		//std::cout << "NORTH " << (isPhysical ? "plating" : "trigger") << std::endl;
	}
	if(m_Direction & SOUTH)
	{
		offsetPos.z -= 0.45;
		lookatPos.z -= 1;
		halfExtents.setZ(0.005f);
		//std::cout << "SOUTH " << (isPhysical ? "plating" : "trigger") << std::endl;
	}
	if(m_Direction & EAST)
	{
		offsetPos.x += 0.45;
		lookatPos.x += 1;
		halfExtents.setX(0.005f);
		//std::cout << "EAST " << (isPhysical ? "plating" : "trigger") << std::endl;
	}
	if(m_Direction & WEST)
	{
		offsetPos.x -= 0.45;
		lookatPos.x -= 1;
		halfExtents.setX(0.005f);

		//std::cout << "WEST " << (isPhysical ? "plating" : "trigger") << std::endl;
	}
	if(m_Direction & UP)
	{
		offsetPos.y += 0.45;
		lookatPos.y += 1;
		halfExtents.setY(0.005f);
		//std::cout << "UP " << (isPhysical ? "plating" : "trigger") << std::endl;
	}
	if(m_Direction & DOWN)
	{
		offsetPos.y -= 0.45;
		lookatPos.y -= 1;
		halfExtents.setY(0.005f);
		//std::cout << "DOWN " << (isPhysical ? "plating" : "trigger") << std::endl;
	}
	m_pAtomEntitySceneNode->setPosition(offsetPos);
	m_pAtomEntitySceneNode->lookAt(lookatPos, Ogre::Node::TS_LOCAL);
	m_pAtomEntitySceneNode->pitch(Ogre::Degree(90));
	
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
		//m_pRigidBody->setCollisionFlags(m_pRigidBody->CF_NO_CONTACT_RESPONSE);
	}
	else
	{
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_STRUCTURE, COLLISION_BUILDRAYCAST);

		//activate gravity
		ToggleGravity();
	}
	InitCollisionShapeDebugDraw(Ogre::ColourValue::Red);

}

void GravPlates::CreateFromBuildPoint()
{
	if(m_IsBuildPoint)
	{
		//first, reset the collision flags for build raycasting
		btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();
		dynamicsWorld.removeRigidBody(m_pRigidBody);
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_STRUCTURE, COLLISION_BUILDRAYCAST);
		//m_pRigidBody->setCollisionFlags(m_pRigidBody->CF_STATIC_OBJECT);
		
		//reset the material
		m_pAtomEntity->setMaterialName("gravplates");
		
		//activate gravity
		ToggleGravity();

		//done
		SetEntityVisible(true);
		m_IsBuildPoint = false;
	}
}

void GravPlates::DestroyToBuildPoint()
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

void GravPlates::Select(ObserverBuild* a_pSelectingObserver)
{
	Atom::Select(a_pSelectingObserver);
	if(m_pAtomEntity)
	{
		if(m_IsBuildPoint)
			m_pAtomEntity->setMaterialName("cell_highlight_material");
		else
			m_pAtomEntity->setMaterialName("gravplates_modulate");
	}
}

void GravPlates::DeSelect(ObserverBuild* a_pSelectingObserver)
{
	Atom::DeSelect(a_pSelectingObserver);
	if(m_pAtomEntity)
	{
		m_pAtomEntity->setMaterialName("gravplates");
	}
}

void GravPlates::ToggleGravity()
{
	//toggle gravity
	if(!m_pSourceMapCell)
	{
		m_pSourceMapCell = MapSuite::GetInstance().GetCellAtCoordsOrNull(m_pAtomRootSceneNode->_getDerivedPosition());
	}
	if(m_pSourceMapCell)
	{
		if(m_GravityActive)
		{
			float gravityForceLeft = m_InitialGravityForce;
			int cellsLeft = m_CellRange;
			MapCell* pCurMapCell = m_pSourceMapCell;
			int gravityDir = m_GravityReversed ? ReverseDir(m_Direction) : m_Direction;
			while(cellsLeft > 0 && gravityForceLeft > 0)
			{
				Ogre::Vector3 curGravForce = GetUnitVectorFromDir(gravityDir) * gravityForceLeft;
				pCurMapCell->RemoveGavityForce(curGravForce);
				//
				pCurMapCell = MapSuite::GetInstance().GetCellInDirOrCreate(pCurMapCell, m_Direction);
				cellsLeft -= 1;
				gravityForceLeft -= m_LinearGravityFalloff;
			}
			m_GravityActive = false;
		}
		else
		{
			float gravityForceLeft = m_InitialGravityForce;
			int cellsLeft = m_CellRange;
			MapCell* pCurMapCell = MapSuite::GetInstance().GetCellInDirOrCreate(m_pSourceMapCell, m_Direction);
			int gravityDir = m_GravityReversed ? m_Direction : ReverseDir(m_Direction);
			Ogre::Vector3 initialGravity = GetUnitVectorFromDir(gravityDir);
			while(cellsLeft > 0 && gravityForceLeft > 0)
			{
				pCurMapCell->AddGravityForce(initialGravity * gravityForceLeft);
				//
				pCurMapCell = MapSuite::GetInstance().GetCellInDirOrCreate(pCurMapCell, m_Direction);
				cellsLeft -= 1;
				gravityForceLeft -= m_LinearGravityFalloff;
			}
			m_GravityActive = true;
		}
	}
}

void GravPlates::SetGravityRange(int a_NewRange)
{
	bool reenableGrav = m_GravityActive;
	if(reenableGrav)
		ToggleGravity();
	m_CellRange = a_NewRange;
	if(reenableGrav)
		ToggleGravity();
}

void GravPlates::SetGravityForce(float a_NewForce)
{
	bool reenableGrav = m_GravityActive;
	if(reenableGrav)
		ToggleGravity();
	m_InitialGravityForce = a_NewForce;
	if(reenableGrav)
		ToggleGravity();
}

void GravPlates::SetLinearGravityFalloff(float a_NewLinearFallof)
{
	bool reenableGrav = m_GravityActive;
	if(reenableGrav)
		ToggleGravity();
	m_LinearGravityFalloff = a_NewLinearFallof;
	if(reenableGrav)
		ToggleGravity();
}
