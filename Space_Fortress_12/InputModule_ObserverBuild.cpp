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

		btCollisionWorld::ClosestRayResultCallback rayCallback(startPos, startPos + rayDir * btScalar(m_CellBuildRange));
		rayCallback.m_collisionFilterGroup = COLLISION_BUILDRAYCAST;
		rayCallback.m_collisionFilterMask = COLLISION_GIRDER;
		if(m_BuildExpansion)
			rayCallback.m_collisionFilterMask = COLLISION_BUILDPOINT;

		btDiscreteDynamicsWorld& bulletWorld = Application::StaticGetDynamicsWorld();
		bulletWorld.rayTest(startPos, startPos + rayDir * btScalar(m_CellBuildRange), rayCallback);
		Atom* pHitAtom = NULL;
		if(rayCallback.hasHit())
		{
			Ogre::Vector3 drawPos = camPos + camDir * Ogre::Real(m_CellBuildRange) * rayCallback.m_closestHitFraction;
			//DebugDrawer::getSingleton().drawSphere(drawPos, 0.1f, Ogre::ColourValue::Red);
			EffectManager::GetSingleton().CacheSphere(new CachedSphere(drawPos, 0.01f, Ogre::ColourValue::Red));
			
			const btCollisionObject* pHitObj = rayCallback.m_collisionObject;
			pHitAtom = (Atom*)pHitObj->getUserPointer();
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
			if(m_pCurrentlyTargettedAtom)
			{
				if(m_pCurrentlyTargettedAtom->GetAtomType() == Atom::STRUCTURE && ((Structure*)m_pCurrentlyTargettedAtom)->IsBuildPoint())
				{
					m_pCurrentlyTargettedAtom->SetEntityVisible(false);
				}
				m_pCurrentlyTargettedAtom->StopFlashingColour();
			}
			a_pNewAtom->SetFlashingColour(Ogre::ColourValue::Green);
			m_pCurrentlyTargettedAtom = a_pNewAtom;
			m_pCurrentlyTargettedAtom->SetEntityVisible();
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
