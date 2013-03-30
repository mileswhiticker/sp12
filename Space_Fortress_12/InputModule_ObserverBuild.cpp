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

ObserverBuild::ObserverBuild(Mob* a_pOwnedMob, Client* a_pOwnedClient)
:	InputModule(a_pOwnedMob, a_pOwnedClient)
,	m_pCurrentlyTargettedAtom(NULL)
,	m_CellBuildRange(1)
,	m_BuildExpansion(true)
,	m_TargetStructureTypes(0)
{
	m_pOwnedMob = a_pOwnedMob;
}

void ObserverBuild::Update(float a_DeltaT)
{
	//DebugDrawer::getSingleton().drawSphere(Ogre::Vector3(0,0,0), 10, Ogre::ColourValue::Blue);
	if(m_pOwnedClient->m_pCamera)
	{
		Ogre::Vector3 camPos = m_pOwnedClient->m_pCamera->getDerivedPosition();
		btVector3 startPos = OGRE2BT(camPos);
		Ogre::Vector3 camDir = m_pOwnedClient->m_pCamera->getDerivedOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
		btVector3 rayDir = OGRE2BT(camDir);
		
		//EffectManager::GetSingleton().CacheLine(new CachedLine(Ogre::Vector3(0,0,0), camPos, Ogre::ColourValue::Green));
		EffectManager::GetSingleton().CacheLine(new CachedLine(camPos, camPos + camDir, Ogre::ColourValue::Blue));
		//DebugDrawer::getSingleton().drawLine(Ogre::Vector3::ZERO, Ogre::Vector3::UNIT_X * 10, Ogre::ColourValue::Green);

		btCollisionWorld::AllHitsRayResultCallback rayCallback(startPos, startPos + rayDir * btScalar(m_CellBuildRange));
		rayCallback.m_collisionFilterGroup = COLLISION_BUILDRAYCAST;
		if(m_BuildExpansion)
			rayCallback.m_collisionFilterMask = COLLISION_BUILDPOINT;
		else
			rayCallback.m_collisionFilterMask = COLLISION_STRUCTURE;

		btDiscreteDynamicsWorld& bulletWorld = Application::StaticGetDynamicsWorld();
		bulletWorld.rayTest(startPos, startPos + rayDir * btScalar(m_CellBuildRange), rayCallback);
		Atom* pHitAtom = NULL;
		if(rayCallback.hasHit())
		{
			//rayCallback.m_collisionObjects.quickSort();
			for(int index=0;index<rayCallback.m_collisionObjects.size();index++)
			{
				const btCollisionObject* pHitObj = rayCallback.m_collisionObjects.at(index);
				pHitAtom = (Atom*)pHitObj->getUserPointer();
				//std::cout << index << "/" << rayCallback.m_collisionObjects.size() << " " << pHitAtom << std::endl;
				if(pHitAtom && pHitAtom->GetAtomType() == Atom::STRUCTURE && (!m_TargetStructureTypes || ((Structure*)pHitAtom)->GetStructureType() == m_TargetStructureTypes))
				{
					//m_TargetStructureTypes
					Ogre::Vector3 drawPos = camPos + camDir * Ogre::Real(m_CellBuildRange) * rayCallback.m_closestHitFraction;
					//DebugDrawer::getSingleton().drawSphere(drawPos, 0.1f, Ogre::ColourValue::Red);
					EffectManager::GetSingleton().CacheSphere(new CachedSphere(drawPos, 0.01f, Ogre::ColourValue::Red));
					
					break;
				}

				//reset it, so that if we don't find a matching structure at all the user's selection is reset
				pHitAtom = NULL;
			}
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
		m_pCurrentlyTargettedAtom = NULL;
	}
}

bool ObserverBuild::keyPressed( const OIS::KeyEvent &arg )
{
	switch(arg.key)
	{
	case(OIS::KC_B):
		{
			m_BuildExpansion = !m_BuildExpansion;
			m_BuildExpansion ? std::cout << "buildpoints" << std::endl : std::cout << "existing structures" << std::endl;
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

bool ObserverBuild::keyReleased( const OIS::KeyEvent &arg )
{
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
	}
	return false;
}

bool ObserverBuild::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return false;
}

bool ObserverBuild::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return false;
}
