#include "InputModule_ObserverBuild.hpp"
#include "Client.hpp"
#include "Application.hpp"

#include "CollisionDefines.h"
#include "BtOgreHelper.hpp"
#include "DebugDrawer.h"
#include "EffectManager.hpp"
#include "Cached.hpp"
#include "Girder.hpp"

#include <Ogre\OgreCamera.h>
#include <BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h>
#include <BulletCollision\CollisionDispatch\btCollisionWorld.h>

#include "Human.hpp"
#include "AtomManager.hpp"
#include <BulletDynamics\Dynamics\btRigidBody.h>
#include <OGRE\OgreSceneNode.h>

ObserverBuild::ObserverBuild(Mob* a_pOwnedMob, Client* a_pOwnedClient)
:	InputModule(a_pOwnedMob, a_pOwnedClient)
,	m_pCurrentlyTargettedAtom(NULL)
,	m_CellBuildRange(1)
,	m_BuildExpansion(true)
,	m_TargetStructureTypes(0)
{
	//create a test human
	m_pTestHuman = (Human*)AtomManager::GetSingleton().CreateMob(Mob::HUMAN, Ogre::Vector3::ZERO, NULL, 0 | INSTANTIATE_IMMEDIATELY);
}

void ObserverBuild::Update(float a_DeltaT)
{
	//DebugDrawer::getSingleton().drawSphere(Ogre::Vector3(0,0,0), 10, Ogre::ColourValue::Blue);
	if(m_pOwnedClient && m_pOwnedClient->m_pCamera)
	{
		Ogre::Vector3 camPos = m_pOwnedClient->m_pCamera->getDerivedPosition();
		btVector3 startPos = OGRE2BT(camPos);
		Ogre::Vector3 camDir = m_pOwnedClient->m_pCamera->getDerivedOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
		btVector3 rayDir = OGRE2BT(camDir);
		
		//EffectManager::GetSingleton().CacheLine(new CachedLine(Ogre::Vector3(0,0,0), camPos, Ogre::ColourValue::Green));
		//EffectManager::GetSingleton().CacheLine(new CachedLine(camPos, camPos + camDir, Ogre::ColourValue::Blue));
		//DebugDrawer::getSingleton().drawLine(Ogre::Vector3::ZERO, Ogre::Vector3::UNIT_X * 10, Ogre::ColourValue::Green);

		btCollisionWorld::ClosestRayResultCallback closestHitRayCallback(startPos, startPos + rayDir * btScalar(m_CellBuildRange));
		closestHitRayCallback.m_collisionFilterGroup = COLLISION_BUILDRAYCAST;
		if(m_BuildExpansion)
			closestHitRayCallback.m_collisionFilterMask = COLLISION_BUILDPOINT;
		else
			closestHitRayCallback.m_collisionFilterMask = COLLISION_STRUCTURE;

		btDiscreteDynamicsWorld& bulletWorld = Application::StaticGetDynamicsWorld();
		bulletWorld.rayTest(startPos, startPos + rayDir * btScalar(m_CellBuildRange), closestHitRayCallback);
		Atom* pHitAtom = NULL;
		
		float hitDistScalar = 999;
		if(closestHitRayCallback.hasHit())
		{
			const btCollisionObject* pHitObj = closestHitRayCallback.m_collisionObject;
			pHitAtom = (Atom*)pHitObj->getUserPointer();
			//std::cout << index << "/" << rayCallback.m_collisionObjects.size() << " " << pHitAtom << std::endl;
			if(pHitAtom && pHitAtom->GetAtomType() == Atom::STRUCTURE && (!m_TargetStructureTypes || ((Structure*)pHitAtom)->GetStructureType() == m_TargetStructureTypes))
			{
				//grab the hit dist fraction, so we can draw a sphere there later
				hitDistScalar = closestHitRayCallback.m_closestHitFraction;
			}
			else
			{
				//reset it, so that if we don't find a matching structure at all the user's selection is reset
				pHitAtom = NULL;
			}
		}

		//if the closest result didn't work, raycast again but grab all results and sort through them
		if(!pHitAtom)
		{
			btCollisionWorld::AllHitsRayResultCallback allHitsRayCallback(startPos, startPos + rayDir * btScalar(m_CellBuildRange));
			allHitsRayCallback.m_collisionFilterGroup = COLLISION_BUILDRAYCAST;
			if(m_BuildExpansion)
				allHitsRayCallback.m_collisionFilterMask = COLLISION_BUILDPOINT;
			else
				allHitsRayCallback.m_collisionFilterMask = COLLISION_STRUCTURE;
			bulletWorld.rayTest(startPos, startPos + rayDir * btScalar(m_CellBuildRange), allHitsRayCallback);
			if(allHitsRayCallback.hasHit())
			{
				for(int index=0;index<allHitsRayCallback.m_collisionObjects.size();index++)
				{
					const btCollisionObject* pHitObj = allHitsRayCallback.m_collisionObjects.at(index);
					Atom* pTestAtom = (Atom*)pHitObj->getUserPointer();

					//check if we've hit a valid target
					if(pTestAtom && pTestAtom->GetAtomType() == Atom::STRUCTURE && (!m_TargetStructureTypes || ((Structure*)pTestAtom)->GetStructureType() == m_TargetStructureTypes))
					{
						//it is, now check if it's closer than the previous valid target
						float curDist = allHitsRayCallback.m_hitFractions[index];
						if(curDist < hitDistScalar)
						{
							//if it is, select it then keep checking the rest
							hitDistScalar = curDist;
							pHitAtom = pTestAtom;
						}	
					}
				}
			}
		}

		if(pHitAtom)
		{
			//draw a sphere at the hit point
			Ogre::Vector3 drawPos = camPos + camDir * Ogre::Real(m_CellBuildRange) * hitDistScalar;
			//EffectManager::GetSingleton().CacheSphere(new CachedSphere(drawPos, 0.01f, Ogre::ColourValue::Red));
		}
		SelectNewAtom(pHitAtom);
	}
}

void ObserverBuild::SelectNewAtom(Atom* a_pNewAtom)
{
	if(a_pNewAtom)
	{
		if(a_pNewAtom != m_pCurrentlyTargettedAtom)
		{
			ClearSelectedAtom();
			a_pNewAtom->SetFlashingColour(Ogre::ColourValue::Green);
			m_pCurrentlyTargettedAtom = a_pNewAtom;
			m_pCurrentlyTargettedAtom->SetEntityVisible();
			m_pCurrentlyTargettedAtom->Select(this);
			//std::cout << "new atom targetted" << std::endl;
		}
	}
	else
	{
		ClearSelectedAtom();
	}
}

void ObserverBuild::ClearSelectedAtom()
{
	if(m_pCurrentlyTargettedAtom)
	{
		//std::cout << "clearing selected atom" << std::endl;
		m_pCurrentlyTargettedAtom->StopFlashingColour();
		if(m_pCurrentlyTargettedAtom->GetAtomType() == Atom::STRUCTURE && ((Structure*)m_pCurrentlyTargettedAtom)->IsBuildPoint())
		{
			m_pCurrentlyTargettedAtom->SetEntityVisible(false);
		}
		m_pCurrentlyTargettedAtom->DeSelect(this);
		m_pCurrentlyTargettedAtom = NULL;
	}
}

void ObserverBuild::ForceClearAtomIfSelected(Atom* a_pOtherAtom)
{
	if(a_pOtherAtom == m_pCurrentlyTargettedAtom)
	{
		ClearSelectedAtom();
	}
}

bool ObserverBuild::keyPressed( const OIS::KeyEvent &arg )
{
	switch(arg.key)
	{
	case(OIS::KC_B):
		{
			m_BuildExpansion = !m_BuildExpansion;
			m_BuildExpansion ? std::cout << "Targetting buildpoints" << std::endl : std::cout << "Targetting existing structures" << std::endl;
			return true;
		}
	case(OIS::KC_H):
		{
			btTransform newTransform;
			newTransform.setIdentity();
			Ogre::Vector3 newPos = m_pOwnedMob->m_pAtomRootSceneNode->_getDerivedPosition();
			newTransform.setOrigin( OGRE2BT(newPos) );
			m_pTestHuman->m_pRigidBody->setLinearVelocity(btVector3(0,0,0));
			m_pTestHuman->m_pRigidBody->setWorldTransform(newTransform);
			return true;
		}
	case(OIS::KC_1):
		{
			m_TargetStructureTypes = 0;
			std::cout << "targetting all structures" << std::endl;
			return true;
		}
	case(OIS::KC_2):
		{
			m_TargetStructureTypes = Structure::GIRDER;
			std::cout << "targetting GIRDER" << std::endl;
			return true;
		}
	case(OIS::KC_3):
		{
			m_TargetStructureTypes = Structure::OVERLAYPLATING;
			std::cout << "targetting OVERLAYPLATING" << std::endl;
			return true;
		}
	case(OIS::KC_4):
		{
			m_TargetStructureTypes = Structure::UNDERLAYPLATING;
			std::cout << "targetting UNDERLAYPLATING" << std::endl;
			return true;
		}
	case(OIS::KC_5):
		{
			m_TargetStructureTypes = Structure::LIGHTFIXTURE;
			std::cout << "targetting LIGHTFIXTURE" << std::endl;
			return true;
		}
	case(OIS::KC_6):
		{
			m_TargetStructureTypes = Structure::GRAVPLATES;
			std::cout << "targetting GRAVPLATES" << std::endl;
			return true;
		}
	case(OIS::KC_I):
		{
			if(m_pCurrentlyTargettedAtom && m_pCurrentlyTargettedAtom->GetAtomType() == Atom::STRUCTURE)
			{
				if(((Structure*)m_pCurrentlyTargettedAtom)->IsBuildPoint())
					((Structure*)m_pCurrentlyTargettedAtom)->CreateFromBuildPoint();
				else
					((Structure*)m_pCurrentlyTargettedAtom)->DestroyToBuildPoint();
			}
			return true;
		}
	}
	
	return false;
}

bool ObserverBuild::mouseMoved( const OIS::MouseEvent &arg )
{
	//zmovement represents mousewheel
	//each 120 units represents a 'notch'
	if (arg.state.Z.rel)
	{
		m_CellBuildRange += arg.state.Z.rel / 120;
		if(m_CellBuildRange < 0)
			m_CellBuildRange = 0;
		else if(m_CellBuildRange > 6)
			m_CellBuildRange = 6;
		return true;
	}
	return false;
}
