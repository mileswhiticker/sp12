#include "ObserverBuild.hpp"
#include "Client.hpp"
#include "Application.hpp"
#include "MapSuite.hpp"

#include "CollisionDefines.h"
#include "BtOgreHelper.hpp"
#include "DebugDrawer.h"
#include "EffectManager.hpp"
#include "Cached.hpp"
#include "Turf.hpp"
#include "Events.hpp"
#include "Structure.hpp"

#include <Ogre\OgreCamera.h>
#include <BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h>
#include <BulletCollision\CollisionDispatch\btCollisionWorld.h>
#include <cegui\elements\CEGUIGUISheet.h>

#include "Human.hpp"
#include "AtomManager.hpp"
#include <BulletDynamics\Dynamics\btRigidBody.h>
#include <OGRE\OgreSceneNode.h>

ObserverBuild::ObserverBuild(Mob* a_pOwnedMob, Client* a_pOwnedClient)
:	Component(a_pOwnedMob, a_pOwnedClient)
,	m_CellBuildRange(1)
,	m_BuildExpansion(true)
,	m_TargetStructureTypes(0)
,	m_TargetAtomTypes(0)
,	m_TargetTurfTypes(0)
{
	//create a test human
	//m_pTestHuman = (Human*)AtomManager::GetSingleton().CreateMob(Mob::HUMAN, Ogre::Vector3::ZERO, NULL, 0 | INSTANTIATE_IMMEDIATELY);
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
		closestHitRayCallback.m_collisionFilterGroup = RAYCAST_BUILD;
		if(m_BuildExpansion)
			closestHitRayCallback.m_collisionFilterMask = COLLISION_BUILDPOINT;
		else
			closestHitRayCallback.m_collisionFilterMask = COLLISION_STRUCTURE | COLLISION_TURF;

		btDiscreteDynamicsWorld& bulletWorld = Application::StaticGetDynamicsWorld();
		bulletWorld.rayTest(startPos, startPos + rayDir * btScalar(m_CellBuildRange), closestHitRayCallback);
		Atom* pHitAtom = NULL;
		
		float hitDistScalar = 999;
		if(closestHitRayCallback.hasHit())
		{
			const btCollisionObject* pHitObj = closestHitRayCallback.m_collisionObject;
			pHitAtom = (Atom*)pHitObj->getUserPointer();
			//std::cout << index << "/" << rayCallback.m_collisionObjects.size() << " " << pHitAtom << std::endl;
			if(TrySelect(pHitAtom))
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
			allHitsRayCallback.m_collisionFilterGroup = RAYCAST_BUILD;
			if(m_BuildExpansion)
				allHitsRayCallback.m_collisionFilterMask = COLLISION_BUILDPOINT;
			else
				allHitsRayCallback.m_collisionFilterMask = COLLISION_STRUCTURE | COLLISION_TURF;
			bulletWorld.rayTest(startPos, startPos + rayDir * btScalar(m_CellBuildRange), allHitsRayCallback);
			if(allHitsRayCallback.hasHit())
			{
				for(int index=0;index<allHitsRayCallback.m_collisionObjects.size();index++)
				{
					const btCollisionObject* pHitObj = allHitsRayCallback.m_collisionObjects.at(index);
					Atom* pTestAtom = (Atom*)pHitObj->getUserPointer();

					//check if we've hit a valid target
					if(TrySelect(pTestAtom))
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

bool ObserverBuild::TrySelect(Atom* a_pTestAtom)
{
	if(a_pTestAtom)	//&& !a_pTestAtom->IsBuildpoint()
	{
		if(!m_TargetAtomTypes)
		{
			return true;
		}

		if(a_pTestAtom->GetAtomType() & m_TargetAtomTypes)
		{
			switch(a_pTestAtom->GetAtomType())
			{
			case(Atom::STRUCTURE):
				{
					if((!m_TargetStructureTypes || ((Structure*)a_pTestAtom)->GetStructureType() & m_TargetStructureTypes))
					{
						return true;
					}
					break;
				}
			case(Atom::TURF):
				{
					if((!m_TargetTurfTypes || ((Turf*)a_pTestAtom)->GetTurfType() & m_TargetTurfTypes))
					{
						return true;
					}
					break;
				}
			}
		}
	}
	return false;
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
	case(OIS::KC_1):
		{
			m_TargetAtomTypes = 0;
			//std::cout << "targetting all structures" << std::endl;
			m_pOwnedClient->m_pTopInfoBar->setText("Observer mode (all)");
			return true;
		}
	case(OIS::KC_2):
		{
			m_TargetAtomTypes = Atom::TURF;
			m_TargetStructureTypes = Turf::GIRDER;
			//std::cout << "targetting GIRDER" << std::endl;
			m_pOwnedClient->m_pTopInfoBar->setText("Observer mode (girders)");
			return true;
		}
	case(OIS::KC_3):
		{
			m_TargetAtomTypes = Atom::STRUCTURE;
			m_TargetStructureTypes = Structure::OVERLAYPLATING;
			//std::cout << "targetting OVERLAYPLATING" << std::endl;
			m_pOwnedClient->m_pTopInfoBar->setText("Observer mode (overlay plating)");
			return true;
		}
	case(OIS::KC_4):
		{
			m_TargetAtomTypes = Atom::STRUCTURE;
			m_TargetStructureTypes = Structure::UNDERLAYPLATING;
			//std::cout << "targetting UNDERLAYPLATING" << std::endl;
			m_pOwnedClient->m_pTopInfoBar->setText("Observer mode (underlay plating)");
			return true;
		}
	case(OIS::KC_5):
		{
			m_TargetAtomTypes = Atom::STRUCTURE;
			m_TargetStructureTypes = Structure::LIGHTFIXTURE;
			//std::cout << "targetting LIGHTFIXTURE" << std::endl;
			m_pOwnedClient->m_pTopInfoBar->setText("Observer mode (light fixtures)");
			return true;
		}
	case(OIS::KC_6):
		{
			m_TargetAtomTypes = Atom::STRUCTURE;
			m_TargetStructureTypes = Structure::GRAVPLATES;
			//std::cout << "targetting GRAVPLATES" << std::endl;
			m_pOwnedClient->m_pTopInfoBar->setText("Observer mode (gravity plates)");
			return true;
		}
	/*case(OIS::KC_I):
		{
			if(m_pCurrentlyTargettedAtom && m_pCurrentlyTargettedAtom->GetAtomType() == Atom::STRUCTURE)
			{
				if(((Structure*)m_pCurrentlyTargettedAtom)->IsBuildPoint())
					((Structure*)m_pCurrentlyTargettedAtom)->CreateFromBuildPoint();
				else
					((Structure*)m_pCurrentlyTargettedAtom)->DestroyToBuildPoint();
			}
			return true;
		}*/
	case(OIS::KC_U):
		{
			MapSuite::SaveMapDefaultName();
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

bool ObserverBuild::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	//todo: raycast forward and loop over contexts until a valid one is found
	/*if(m_pCurrentlyTargettedAtom && m_pOwnedMob)
	{
		m_pCurrentlyTargettedAtom->Interact(m_pOwnedMob, this, m_pOwnedMob->GetIntent(), Event::TOGGLEBUILD);
		return true;
	}*/
	return false;
}

//todo: is this needed?
bool ObserverBuild::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return false;
}
