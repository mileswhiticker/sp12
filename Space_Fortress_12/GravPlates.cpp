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

#include "Turf.hpp"

#include "OgreHelper.hpp"
#include "BulletHelper.hpp"
#include "BtOgreHelper.hpp"

#include "MapSuite.hpp"
#include "MapHelper.hpp"
#include "MapCell.hpp"
#include "CollisionDefines.h"
#include "Direction.h"
#include "num2string.h"

GravPlates::GravPlates(Turf* a_pLocTurf, int a_Dir)
:	Structure(a_pLocTurf, a_Dir)
,	m_PercentGravityFalloff(0.1f)
,	m_GravityActive(false)
{
	m_MyStructureType = Structure::GRAVPLATES;
	m_MaterialName = "gravplates";
	m_SelectMaterialName = "gravplates_modulate";
	
	m_FaceQuadrant = ALLQUADRANT;
	//m_StructureLevel = UNDER;
}

void GravPlates::InstantiateStructure(bool a_IsBuildPoint)
{
	//an overlay plate is essentially an "outer cover" for the tile in one of the six cardinal directions
	//system is currently setup to handle any combination of the six, but it probably shouldn't be
	m_IsBuildPoint = a_IsBuildPoint;
	Ogre::SceneManager& sceneManager = GetSceneManager();
	//std::cout << "instantiating OverlayPlating with direction " << m_Direction << std::endl;
	
	//create entity
	m_pAtomEntity = sceneManager.createEntity("gravplates_" + num2string(m_AtomID), "gravplates.mesh");
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
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_BUILDPOINT, RAYCAST_BUILD);

		//todo: is this working?
		//m_pRigidBody->setCollisionFlags(m_pRigidBody->CF_NO_CONTACT_RESPONSE);
	}
	else
	{
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_STRUCTURE, RAYCAST_BUILD);

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
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_STRUCTURE, RAYCAST_BUILD);
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
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_BUILDPOINT, RAYCAST_BUILD);
		
		//reset the material
		m_pAtomEntity->setMaterialName("cell_highlight_material");

		//disable gravity
		SetGravity(false);

		//done
		SetEntityVisible(false);
		m_IsBuildPoint = true;
	}
}

void GravPlates::SetGravity(bool a_Active)
{
	if(m_GravityActive != a_Active)
		ToggleGravity();
}

void GravPlates::ToggleGravity()
{
	//toggle gravity
	if(!m_pCurrentTurf)
	{
		m_pCurrentTurf = MapSuite::GetSingleton().GetTurfAtCoordsOrCreate(m_pAtomRootSceneNode->_getDerivedPosition());
	}
	if(m_pCurrentTurf)
	{
		if(m_GravityActive)
		{
			m_Gravity = Ogre::Vector3::ZERO;
			//
			Turf* pCurTurf = m_pCurrentTurf;
			float percentLeft = 1.0f;
			while(percentLeft > 0)
			{
				pCurTurf->RemoveGravityAffector(this);

				//update atoms in that cell
				for(auto it = pCurTurf->m_AtomContents.begin(); it != pCurTurf->m_AtomContents.end(); ++it)
				{
					(*it)->OnGravityChange();
				}

				//
				pCurTurf = MapSuite::GetSingleton().GetTurfInDirOrCreate(pCurTurf, m_Direction);
				percentLeft -= m_PercentGravityFalloff;
			}
			m_GravityActive = true;
		}
		else
		{
			m_Gravity = GetUnitVectorFromDir(ReverseDir(m_Direction)) * 9.8f;
			//
			Turf* pCurTurf = m_pCurrentTurf;
			float percentLeft = 1.0f;
			while(percentLeft > 0)
			{
				pCurTurf->AddGravityAffector(this, percentLeft);
				
				//update atoms in that cell
				for(auto it = pCurTurf->m_AtomContents.begin(); it != pCurTurf->m_AtomContents.end(); ++it)
				{
					(*it)->OnGravityChange();
				}

				//
				pCurTurf = MapSuite::GetSingleton().GetTurfInDirOrCreate(pCurTurf, m_Direction);
				percentLeft -= m_PercentGravityFalloff;
			}
			m_GravityActive = true;
		}
	}
}

/*
void GravPlates::SetGravityRange(int a_NewRange)
{
	bool reenableGrav = m_GravityActive;
	if(reenableGrav)
		ToggleGravity();
	m_CellRange = a_NewRange;
	if(reenableGrav)
		ToggleGravity();
}

void GravPlates::SetGravityAccel(float a_NewAccel)
{
	bool reenableGrav = m_GravityActive;
	if(reenableGrav)
		ToggleGravity();
	m_InitialGravityAccel = a_NewAccel;
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
*/