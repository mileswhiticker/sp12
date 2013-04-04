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

#include "DebugDrawer.h"

Atom::Atom()
:	m_pAtomSceneNode(NULL)
,	m_pAtomEntity(NULL)
,	m_ColourModulateLevel(0)
,	m_ModulateChangeDir(0)
,	m_pRigidBody(NULL)
,	m_pCollisionShape(NULL)
,	m_MyAtomType(UNKNOWN)
,	m_Direction(0)
,	m_UseRigidbodyPosition(true)
{
	//
}

Atom::~Atom()
{
	Ogre::SceneManager& sceneManager = GetSceneManager();
	btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();

	for(auto it = m_pSelectingObservers.begin(); it != m_pSelectingObservers.end(); ++it)
	{
		(*it)->ForceClearAtomIfSelected(this);
	}
	
	//clear rigidbody
	if(m_pRigidBody)
	{
		dynamicsWorld.removeRigidBody(m_pRigidBody);
		delete m_pRigidBody;
	}

	//clear entity
	if(m_pAtomEntity)
	{
		SetEntityVisible(false);
		sceneManager.destroyEntity(m_pAtomEntity);
	}

	//clear scenenode
	if(m_pAtomSceneNode)
	{
		if(m_pAtomSceneNode->getParentSceneNode())
		{
			m_pAtomSceneNode->getParentSceneNode()->removeChild(m_pAtomSceneNode);
		}
		sceneManager.destroySceneNode(m_pAtomSceneNode);
	}
}

void Atom::Update(float a_DeltaT)
{
	//phys updates
	if(m_UseRigidbodyPosition && m_pRigidBody && m_pAtomSceneNode)
	{
		btTransform& transform = m_pRigidBody->getWorldTransform();
		btVector3& origin = transform.getOrigin();
		btQuaternion& rotation = transform.getRotation();

		m_pAtomSceneNode->setPosition(origin.getX(), origin.getY(), origin.getZ());
		m_pAtomSceneNode->setOrientation(rotation.w(), rotation.x(), rotation.y(), rotation.z());
	}

	//--- highlights shader ---//

	//chance to randomly turn on for testing
	/*if(!m_ModulateChangeDir)
	{
		int val = iRand(1000);
		if(val == 0)
		{
			SetFlashingColour(Ogre::ColourValue(1,0,0,1));
		}
		else if(val == 1)
		{
			SetFlashingColour(Ogre::ColourValue(0,1,0,1));
		}
		else if(val == 2)
		{
			SetFlashingColour(Ogre::ColourValue(0,0,1,1));
		}
	}*/

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

		//stop flashing after a while
		/*if(!iRand(250))
		{
			StopFlashingColour();
		}*/

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
			if(pEntitySceneNode != m_pAtomSceneNode)
			{
				if(pEntitySceneNode)
					pEntitySceneNode->detachObject(m_pAtomEntity);
				m_pAtomSceneNode->attachObject(m_pAtomEntity);
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

void Atom::TargetcastIntercept()
{
	//
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
