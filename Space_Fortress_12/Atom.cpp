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

#include "EffectManager.hpp"
#include "Cached.hpp"
#include "DebugDrawer.h"
#include "MapCell.hpp"
#include "MapSuite.hpp"


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
{
	m_pAtomRootSceneNode = NewSceneNode();
	m_pAtomRootSceneNode->setPosition(a_Pos);
	m_pAtomEntitySceneNode = m_pAtomRootSceneNode->createChildSceneNode();
}

Atom::~Atom()
{
	Ogre::SceneManager& sceneManager = GetSceneManager();
	btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();

	for(auto it = m_pSelectingObservers.begin(); it != m_pSelectingObservers.end(); ++it)
	{
		(*it)->ForceClearAtomIfSelected(this);
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

		if(m_UseRigidbodyPosition && m_pAtomRootSceneNode)
		{
			m_pAtomRootSceneNode->setPosition(origin.getX(), origin.getY(), origin.getZ());
			m_pAtomRootSceneNode->setOrientation(rotation.w(), rotation.x(), rotation.y(), rotation.z());
		}
		if(m_pCachedCube)
		{
			m_pCachedCube->pos = BT2OGRE(origin);
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
	
	//--- Gravity ---//
	
	if(m_UsesGravity && m_pRigidBody)
	{
		//update the map cell we're currently in
		Ogre::Vector3 curPos = BT2OGRE( m_pRigidBody->getWorldTransform().getOrigin() );
		if(!m_pSourceMapCell || m_pSourceMapCell->m_Position.squaredDistance(curPos) > 0.25)
		{
			//if we've moved far enough away from the current cell (or it doesn't exist) get a new one
			m_pSourceMapCell = MapSuite::GetInstance().GetCellAtCoordsOrNull(curPos + Ogre::Vector3(0.5f,0.5f,0.5f));
		}

		//apply gravity of the current cell
		if(m_pSourceMapCell)
		{
			m_pRigidBody->applyCentralForce( OGRE2BT(m_pSourceMapCell->GetGravity() * a_DeltaT) );
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

bool Atom::ChangeDirection(int a_NewDir)
{
	m_Direction = a_NewDir;
	return true;
}

void Atom::Select(ObserverBuild* a_pSelectingObserver)
{
	if(a_pSelectingObserver)
	{
		m_pSelectingObservers.insert(m_pSelectingObservers.end(), a_pSelectingObserver);
	}
}

void Atom::DeSelect(ObserverBuild* a_pSelectingObserver)
{
	if(a_pSelectingObserver)
	{
		m_pSelectingObservers.erase(a_pSelectingObserver);
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
