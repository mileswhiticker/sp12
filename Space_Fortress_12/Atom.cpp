#include "Atom.hpp"

#include <OGRE\OgreEntity.h>
#include <OGRE\OgreSubEntity.h>
#include <OGRE\OgreVector4.h>
#include <OGRE\OgreSceneNode.h>

#include "RandHelper.h"

#include <BulletCollision\CollisionShapes\btBoxShape.h>
#include <BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>
#include <LinearMath\btDefaultMotionState.h>

#include "DebugDrawer.h"
#include "BtOgreHelper.hpp"

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
