#include "Atom.hpp"
#include "InputModule_ObserverBuild.hpp"

#include <OGRE\OgreEntity.h>
#include <OGRE\OgreSubEntity.h>
#include <OGRE\OgreVector4.h>
#include <OGRE\OgreSceneNode.h>


#include <BulletCollision\CollisionShapes\btBoxShape.h>
#include <BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>
#include <LinearMath\btDefaultMotionState.h>

#include "OgreHelper.hpp"
#include "BulletHelper.hpp"
#include "RandHelper.h"
#include "BtOgreHelper.hpp"
#include "Mob.hpp"

#include "EffectManager.hpp"
#include "Cached.hpp"
#include "DebugDrawer.h"
#include "MapCell.hpp"
#include "MapSuite.hpp"
#include "Events.hpp"

Atom::Atom(Ogre::Vector3 a_Pos, int a_Dir)
:	m_pAtomEntitySceneNode(NULL)
,	m_pAtomRootSceneNode(NULL)
,	m_pAtomEntity(NULL)
,	m_ColourModulateLevel(0)
,	m_ModulateChangeDir(0)
,	m_pRigidBody(NULL)
,	m_pCollisionShape(NULL)
,	m_MyAtomType(UNKNOWN)
,	m_Direction(a_Dir)
,	m_UseRigidbodyPosition(true)
,	m_pCachedCube(NULL)
,	m_pSourceMapCell(NULL)
,	m_UsesGravity(true)
,	m_RegularPositionUpdates(false)
,	m_tLeftUpdateCell(0)
{
	m_pAtomRootSceneNode = NewSceneNode();
	m_pAtomRootSceneNode->setPosition(a_Pos);
	m_pAtomEntitySceneNode = m_pAtomRootSceneNode->createChildSceneNode();
}

Atom::~Atom()
{
	Ogre::SceneManager& sceneManager = GetSceneManager();
	btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();

	while(m_SelectingInputModules.size())
	{
		(*m_SelectingInputModules.begin())->ForceClearAtomIfSelected(this);
	}
	
	//clear physics
	if(m_pRigidBody)
	{
		dynamicsWorld.removeRigidBody(m_pRigidBody);
		delete m_pRigidBody;
	}
	if(m_pCollisionShape)
	{
		delete m_pCollisionShape;
	}
	if(m_pCachedCube)
	{
		EffectManager::GetSingleton().ClearCacheCube(m_pCachedCube);
		delete m_pCachedCube;
	}

	//clear entity
	if(m_pAtomEntity)
	{
		SetEntityVisible(false);
		sceneManager.destroyEntity(m_pAtomEntity);
	}
	
	//clear scenenode
	if(m_pAtomEntitySceneNode)
	{
		if(m_pAtomEntitySceneNode->getParentSceneNode())
		{
			m_pAtomEntitySceneNode->getParentSceneNode()->removeChild(m_pAtomEntitySceneNode);
		}
		sceneManager.destroySceneNode(m_pAtomEntitySceneNode);
	}
	if(m_pAtomRootSceneNode)
	{
		if(m_pAtomRootSceneNode->getParentSceneNode())
		{
			m_pAtomRootSceneNode->getParentSceneNode()->removeChild(m_pAtomRootSceneNode);
		}
		sceneManager.destroySceneNode(m_pAtomRootSceneNode);
	}
}

void Atom::Update(float a_DeltaT)
{
	//phys updates
	if(m_pRigidBody)
	{
		btTransform& transform = m_pRigidBody->getWorldTransform();
		btVector3& origin = transform.getOrigin();
		btQuaternion& rotation = transform.getRotation();
		
		//hard limit on velocity to 30 m/s
		if(m_pRigidBody->getLinearVelocity().length2() > 900.0f)
		{
			btVector3 newVelocity = m_pRigidBody->getLinearVelocity().normalized() * 30.0f;
			m_pRigidBody->setLinearVelocity(newVelocity);
		}

		if(m_UseRigidbodyPosition && m_pAtomRootSceneNode)
		{
			m_pAtomRootSceneNode->setPosition(origin.getX(), origin.getY(), origin.getZ());
			m_pAtomRootSceneNode->setOrientation(rotation.w(), rotation.x(), rotation.y(), rotation.z());
			//m_pAtomEntitySceneNode->setOrientation(Ogre::Quaternion::IDENTITY);
		}
		if(m_pCachedCube)
		{
			m_pCachedCube->pos = BT2OGRE(origin);
		}
	}
	
	//--- update current map cell ---//
	
	m_tLeftUpdateCell -= a_DeltaT;
	if(m_RegularPositionUpdates && m_pAtomRootSceneNode && m_tLeftUpdateCell <= 0)
	{
		m_tLeftUpdateCell = 0.5f;
		//update the map cell we're currently in
		Ogre::Vector3 curPos = m_pAtomRootSceneNode->getPosition();// + Ogre::Vector3(0.5f,0.5f,0.5f);
		if(curPos.x > 0)
			curPos.x += 0.5f;
		else
			curPos.x -= 0.5f;
		if(curPos.y > 0)
			curPos.y += 0.5f;
		else
			curPos.y -= 0.5f;
		if(curPos.z > 0)
			curPos.z += 0.5f;
		else
			curPos.z -= 0.5f;
		MapCell* pNewMapCell = MapSuite::GetInstance().GetCellAtCoordsOrNull(curPos);
		bool check = false;
		if(pNewMapCell != m_pSourceMapCell)
		{
			//std::cout << "cells don't match, updating..." << std::endl;
			check = true;
		}
		else if(m_pSourceMapCell && m_pSourceMapCell->m_Position.squaredDistance(curPos) > 1)
		{
			//std::cout << "dist too far from previous cell, updating..." << std::endl;
			//check = true;
		}

		if(check)
		{
			/*if(((Mob*)(this))->GetMobType() == Mob::HUMAN)
				std::cout << "human position update" << std::endl;
			else
				std::cout << "observer position update" << std::endl;*/
			if(m_pSourceMapCell)
			{
				m_pSourceMapCell->AtomLeaveCell(this);
				//std::cout << "	leaving cell" << std::endl;
			}
			m_pSourceMapCell = pNewMapCell;
			if(pNewMapCell)
			{
				m_pSourceMapCell->AtomEnterCell(this);
				//std::cout << "	cell " << m_pSourceMapCell->m_Position << " | " << m_pAtomRootSceneNode->getPosition() << std::endl;
			}
			else
			{
				//std::cout << "	environment reset" << std::endl;
				ResetEnvironment();
			}
		}
	}

	//--- highlights shader ---//
	
	if(m_pAtomEntity && m_ModulateChangeDir)
	{
		m_ColourModulateLevel += a_DeltaT * m_ModulateChangeDir;
		if(m_ColourModulateLevel > 0.75f)
		{
			m_ModulateChangeDir = -1;
			m_ColourModulateLevel = 0.75f;
		}
		else if(m_ColourModulateLevel < 0.25f)
		{
			m_ModulateChangeDir = 1;
			m_ColourModulateLevel = 0.25f;
		}

		//pass in the updated colour level to the shader
		unsigned int numSubEntities = m_pAtomEntity->getNumSubEntities();
		//std::cout << m_ColourModulateLevel << std::endl;
		for(unsigned int i=0; i<numSubEntities; ++i)
		{
			//only need to give it a scalar, but for some reason can only pass in float4 params?
			m_pAtomEntity->getSubEntity(i)->setCustomParameter(1, Ogre::Vector4(m_ColourModulateLevel, 1, 1, 1));
		}
	}
}

void Atom::SetFlashingColour(Ogre::ColourValue a_FlashColour)
{
	if(m_pAtomEntity)
	{
		m_ModulateChangeDir = 1;
		
		//pass in the new flash colour value to the shader
		unsigned int numSubEntities = m_pAtomEntity->getNumSubEntities();
		for(unsigned int i=0; i<numSubEntities; ++i)
		{
			m_pAtomEntity->getSubEntity(i)->setCustomParameter(2, Ogre::Vector4(a_FlashColour.r, a_FlashColour.g, a_FlashColour.b, a_FlashColour.a));
		}
	}
}

void Atom::StopFlashingColour()
{
	m_ModulateChangeDir = 0;
	m_ColourModulateLevel = 0;

	//reset the shader value
	if(m_pAtomEntity)
	{
		unsigned int numSubEntities = m_pAtomEntity->getNumSubEntities();
		for(unsigned int i=0; i<numSubEntities; ++i)
		{
			m_pAtomEntity->getSubEntity(i)->setCustomParameter(1, Ogre::Vector4(m_ColourModulateLevel, 1, 1, 1));
		}
	}
}

void Atom::SetEntityVisible(bool a_Visible)
{
	if(m_pAtomEntity)
	{
		if(a_Visible)
		{
			Ogre::SceneNode* pEntitySceneNode = m_pAtomEntity->getParentSceneNode();
			if(pEntitySceneNode != m_pAtomEntitySceneNode)
			{
				if(pEntitySceneNode)
					pEntitySceneNode->detachObject(m_pAtomEntity);
				m_pAtomEntitySceneNode->attachObject(m_pAtomEntity);
			}
		}
		else
		{
			Ogre::SceneNode* pEntitySceneNode = m_pAtomEntity->getParentSceneNode();
			if(pEntitySceneNode)
			{
				pEntitySceneNode->detachObject(m_pAtomEntity);
			}
		}
	}
}

Atom::AtomType Atom::GetAtomType()
{
	return m_MyAtomType;
}

MapCell* Atom::GetSourceMapCell()
{
	return m_pSourceMapCell;
}

bool Atom::ChangeDirection(int a_NewDir)
{
	m_Direction = a_NewDir;
	return true;
}

void Atom::Select(InputModule* a_pSelectingInputModule)
{
	if(a_pSelectingInputModule)
	{
		SetFlashingColour(Ogre::ColourValue::Green);
		SetEntityVisible();
		m_SelectingInputModules.insert(a_pSelectingInputModule);
	}
}

void Atom::DeSelect(InputModule* a_pSelectingInputModule)
{
	if(a_pSelectingInputModule)
	{
		m_SelectingInputModules.erase(a_pSelectingInputModule);
		StopFlashingColour();
	}
}

int Atom::GetDirection()
{
	return m_Direction;
}

void Atom::InitCollisionShapeDebugDraw(Ogre::ColourValue a_ColourVal)
{
	//debug draw the collision box
	if(m_pCollisionShape)
	{
		btVector3 halfExtents = m_pCollisionShape->getHalfExtentsWithMargin();
		Ogre::Vector3 verts[8] = {\
			Ogre::Vector3(-halfExtents.getX(), halfExtents.getY(), halfExtents.getZ()),\
			Ogre::Vector3(halfExtents.getX(), halfExtents.getY(), halfExtents.getZ()),\
			Ogre::Vector3(halfExtents.getX(), -halfExtents.getY(), halfExtents.getZ()),\
			Ogre::Vector3(-halfExtents.getX(), -halfExtents.getY(), halfExtents.getZ()),\
		
			Ogre::Vector3(halfExtents.getX(), halfExtents.getY(), -halfExtents.getZ()),\
			Ogre::Vector3(-halfExtents.getX(), halfExtents.getY(), -halfExtents.getZ()),\
			Ogre::Vector3(-halfExtents.getX(), -halfExtents.getY(), -halfExtents.getZ()),\
			Ogre::Vector3(halfExtents.getX(), -halfExtents.getY(), -halfExtents.getZ()),\
		};
		m_pCachedCube = new CachedCube(m_pAtomEntitySceneNode->_getDerivedPosition(), verts, a_ColourVal);
		EffectManager::GetSingleton().CacheCube(m_pCachedCube);
	}
}

void Atom::Interact(Atom* a_pSourceAtom, InputModule* a_pSourceModule, int a_Intent, int a_Type)
{
	switch(a_Type)
	{
	case(Event::EX_ACT):
		{
			//explosion
			break;
		}
	case(Event::EMP_ACT):
		{
			//emp
			break;
		}
	default:
		{
			//no special type of interaction, just use the default interactions for each intent
			if(a_Intent)
			{
				//hit this atom with a_pSourceAtom 
			}
			else
			{
				//nothing happens
			}
			break;
		}
	}
}

void Atom::CancelInteract(Atom* a_pSource, int a_Intent, int a_Type)
{
	//in case we have a case where you have to continually activate something
	//
}

//returns true if gravity has actually changed
bool Atom::OnGravityChange()
{
	if(m_UsesGravity && m_pRigidBody)
	{
		btVector3 oldGravity = m_pRigidBody->getGravity();
		if(m_pSourceMapCell)
		{
			btVector3 newGravity = OGRE2BT(m_pSourceMapCell->GetGravity());
			if(newGravity != oldGravity)
			{
				//apply gravity of the current cell
				m_pRigidBody->setGravity( OGRE2BT(m_pSourceMapCell->GetGravity()) );
				std::cout << "gravity updated" << std::endl;
				return true;
			}
		}
		else if(oldGravity.length2())
		{
			m_pRigidBody->setGravity(btVector3(0,0,0));
			std::cout << "gravity nulled" << std::endl;
			return true;
		}
		return false;
	}
	return false;
}

void Atom::ResetEnvironment()
{
	//todo: grab the ambient environmental values for space
	OnGravityChange();
}
