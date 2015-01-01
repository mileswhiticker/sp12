#include "Mob.hpp"

#include "InputModule.hpp"
#include "Client.hpp"
#include "MapSuite.hpp"
//#include "MapCell.hpp"
#include "Application.hpp"
#include "Turf.hpp"
#include "Mob.hpp"
#include "Object.hpp"

#include <LinearMath\btVector3.h>
#include <BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h>
#include <BulletCollision\CollisionDispatch\btCollisionWorld.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>

#include "EffectManager.hpp"
#include "Cached.hpp"
#include "CollisionDefines.h"
#include "OgreHelper.hpp"
#include "RandHelper.h"
#include "BtOgreHelper.hpp"
#include "DebugDrawer.h"

Mob::Mob(Ogre::Vector3 a_StartPos, int a_Direction)
:	Atom(a_StartPos, a_Direction)
,	m_pPossessingClient(NULL)
,	m_MyMobType(UNKNOWN)
,	m_tLeftUprightOrientation(0)
,	m_TargetStandingOrientation(btQuaternion(0,0,0))
,	m_tleftNextGroundRaycast(0.5f)
,	m_IsOnGround(false)
,	m_CameraModelOffset(Ogre::Vector3::ZERO)
,	m_Intent(HELP)
,	m_pHeldObject(NULL)
,	m_pCurrentRaycastingInputModule(NULL)
,	m_tLeftInteractRaycast(3)
,	m_InteractRaycastInterval(0.1f)
{
	m_MyAtomType = Atom::MOB;
	//btQuaternion(btVector3(0,1,0), 0);
	m_DefaultPhysicsGroup = COLLISION_MOB;
	m_DefaultPhysicsMask = COLLISION_OBJ|COLLISION_MOB|COLLISION_STRUCTURE;
}

Mob::~Mob()
{
	//todo:
	//clear input modules
	//clear bullet rigidbodies, collision shapes
	//clear ogre entities, scenenodes
}

void Mob::Update(float a_DeltaT)
{
	Atom::Update(a_DeltaT);
	
	//raycast forward if we need to
	m_tLeftInteractRaycast -= a_DeltaT;
	if(m_tLeftInteractRaycast <= 0)
	{
		m_tLeftInteractRaycast = m_InteractRaycastInterval;
		RaycastForward();
	}

	//InputModules
	for(auto it = m_ActiveInputModules.begin(); it != m_ActiveInputModules.end(); ++it)
	{
		(*it)->Update(a_DeltaT);
	}

	//if we're "falling", raycast down to see if we're on the "ground"
	m_tleftNextGroundRaycast -= a_DeltaT;
	if(m_pRigidBody && m_UsesGravity && m_pRigidBody->getGravity().length2() > 0 && m_tleftNextGroundRaycast <= 0)
	{
		//todo: this whole method is a little crude
		m_tleftNextGroundRaycast = 0.5f;
		UpdateOnGround();
	}

	UpdateOrientation(a_DeltaT);
}

//InputModule* m_pCurrentRaycastingInputModule;
void Mob::RaycastForward()
{
	//DebugDrawer::getSingleton().drawSphere(Ogre::Vector3(0,0,0), 10, Ogre::ColourValue::Blue);
	if(m_pCurrentRaycastingInputModule && m_pPossessingClient && m_pPossessingClient->m_pCamera)
	{
		float interactRange = m_pCurrentRaycastingInputModule->GetInteractRange();
		if(!interactRange)
			return;
		
		Ogre::Vector3 camPos = m_pPossessingClient->m_pCamera->getDerivedPosition();
		btVector3 startPos = OGRE2BT(camPos);
		Ogre::Vector3 camDir = m_pPossessingClient->m_pCamera->getDerivedOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
		btVector3 rayDir = OGRE2BT(camDir);
		
		//EffectManager::GetSingleton().CacheLine(new CachedLine(Ogre::Vector3(0,0,0), camPos, Ogre::ColourValue::Green));
		//EffectManager::GetSingleton().CacheLine(new CachedLine(camPos, camPos + camDir, Ogre::ColourValue::Blue));
		//DebugDrawer::getSingleton().drawLine(Ogre::Vector3::ZERO, Ogre::Vector3::UNIT_X * 10, Ogre::ColourValue::Green);

		btCollisionWorld::ClosestRayResultCallback closestHitRayCallback(startPos, startPos + rayDir * btScalar(interactRange));
		closestHitRayCallback.m_collisionFilterGroup = m_pCurrentRaycastingInputModule->GetInteractFilterGroup();
		closestHitRayCallback.m_collisionFilterMask = m_pCurrentRaycastingInputModule->GetInteractFilterMask();

		btDiscreteDynamicsWorld& bulletWorld = Application::StaticGetDynamicsWorld();
		
		Atom* pHitAtom = NULL;
		std::vector<Atom*> interactableAtoms;
		
		float hitDistScalar = 999;

		//this grabs the closest viable result... may as well just grab all of the results
		/*bulletWorld.rayTest(startPos, startPos + rayDir * btScalar(interactRange), closestHitRayCallback);
		if(closestHitRayCallback.hasHit())
		{
			const btCollisionObject* pHitObj = closestHitRayCallback.m_collisionObject;
			pHitAtom = (Atom*)pHitObj->getUserPointer();
			//std::cout << index << "/" << rayCallback.m_collisionObjects.size() << " " << pHitAtom << std::endl;
			if(m_pCurrentRaycastingInputModule->TryInteract(pHitAtom))
			{
				//grab the hit dist fraction, so we can draw a sphere there later
				hitDistScalar = closestHitRayCallback.m_closestHitFraction;
			}
			else
			{
				//reset it, so that if we don't find a matching structure at all the user's selection is reset
				pHitAtom = NULL;
			}
		}*/

		//grab all results and pass them over to the input module to sort through
		//bullet does not sort these by distance! we have to do it manually here
		if(!pHitAtom)
		{
			btCollisionWorld::AllHitsRayResultCallback allHitsRayCallback(startPos, startPos + rayDir * btScalar(interactRange));
			allHitsRayCallback.m_collisionFilterGroup = closestHitRayCallback.m_collisionFilterGroup;
			allHitsRayCallback.m_collisionFilterMask = closestHitRayCallback.m_collisionFilterMask;

			bulletWorld.rayTest(startPos, startPos + rayDir * btScalar(interactRange), allHitsRayCallback);
			if(allHitsRayCallback.hasHit())
			{
				for(int index=0;index<allHitsRayCallback.m_collisionObjects.size();index++)
				{
					const btCollisionObject* pHitObj = allHitsRayCallback.m_collisionObjects.at(index);
					Atom* pTestAtom = (Atom*)pHitObj->getUserPointer();

					//interactableAtoms.push_back(pTestAtom);

					//check if it's closer than the previous target
					float curDist = allHitsRayCallback.m_hitFractions[index];
					if(curDist < hitDistScalar)
					{
						//check if we've hit a valid target
						if(m_pCurrentRaycastingInputModule->TryInteract(pTestAtom, false))
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
			/*float myVar;
			myVar = 1;
			//draw a sphere at the hit point
			Ogre::Vector3 drawPos = camPos + camDir * Ogre::Real(interactRange) * hitDistScalar;
			EffectManager::GetSingleton().CacheSphere(new CachedSphere(drawPos, 0.01f, Ogre::ColourValue::Red));*/
		}

		//there's bit of cleanup to do in this function... if i ever get it working satisfactorily
		if(interactableAtoms.size() > 0)
		{
			m_pCurrentRaycastingInputModule->TryInteract(interactableAtoms);
		}
		else
		{
			m_pCurrentRaycastingInputModule->TryInteract(pHitAtom, true);
		}
	}
}

bool Mob::ConnectClient(Client* a_pNewClient)
{
	if(!m_pPossessingClient)
	{
		m_pPossessingClient = a_pNewClient;
		m_pPossessingClient->m_pPossessedMob = this;
		m_pPossessingClient->ResetCamera();

		RegisterKeyListener(this);
		RegisterMouseListener(this);
		
		return true;
	}
	return false;
}

void Mob::InstantiateAtom()
{
	if(m_pRigidBody)
	{
		m_pRigidBody->applyForce(btVector3(btScalar(fRand(-2,2)), btScalar(fRand(-2,2)), btScalar(fRand(-2,2))), btVector3(fRand(-0.1f,0.1f), fRand(-0.1f,0.1f), fRand(-0.1f,0.1f)));
	}
}

bool Mob::DisconnectClient()
{
	if(m_pPossessingClient)
	{
		m_pPossessingClient = NULL;

		return true;
	}
	return false;
}

bool Mob::keyPressed( const OIS::KeyEvent &arg )
{
	for(auto it = m_ActiveInputModules.begin(); it != m_ActiveInputModules.end(); ++it)
	{
		(*it)->keyPressed(arg);
	}

	return false;
}

bool Mob::keyReleased( const OIS::KeyEvent &arg )
{
	for(auto it = m_ActiveInputModules.begin(); it != m_ActiveInputModules.end(); ++it)
	{
		(*it)->keyReleased(arg);
	}

	return false;
}

bool Mob::mouseMoved( const OIS::MouseEvent &arg )
{
	for(auto it = m_ActiveInputModules.begin(); it != m_ActiveInputModules.end(); ++it)
	{
		(*it)->mouseMoved(arg);
	}

	return false;
}

bool Mob::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	for(auto it = m_ActiveInputModules.begin(); it != m_ActiveInputModules.end(); ++it)
	{
		(*it)->mousePressed(arg, id);
	}

	return false;
}

bool Mob::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	for(auto it = m_ActiveInputModules.begin(); it != m_ActiveInputModules.end(); ++it)
	{
		(*it)->mouseReleased(arg, id);
	}

	return false;
}

Mob::MobType Mob::GetMobType()
{
	return m_MyMobType;
}

bool Mob::IsOnGround()
{
	return m_IsOnGround;
}

Ogre::Vector3 Mob::GetCameraModelOffset()
{
	return m_CameraModelOffset;
}

bool Mob::UpdateOnGround()
{
	if(m_pRigidBody)
	{
		btTransform worldTransform = m_pRigidBody->getWorldTransform();
		btVector3 startPos = worldTransform.getOrigin();
		btVector3 castDir = m_pRigidBody->getGravity().normalized() * 0.5f;
		btCollisionWorld::ClosestRayResultCallback closestHitRayCallback(startPos, startPos + castDir);
		closestHitRayCallback.m_collisionFilterGroup = RAYCAST_BUILD;		//todo: this needs its own collision define, but this one works for now
		closestHitRayCallback.m_collisionFilterMask = COLLISION_STRUCTURE | COLLISION_OBJ | COLLISION_MOB;
		
		btDiscreteDynamicsWorld& bulletWorld = Application::StaticGetDynamicsWorld();
		bulletWorld.rayTest(startPos, startPos + castDir, closestHitRayCallback);
		Atom* pHitAtom = NULL;

		if(closestHitRayCallback.hasHit())
		{
			/*if(!m_IsOnGround)
				m_tLeftUprightOrientation = 0;*/
			m_IsOnGround = true;
		}
		else
		{
			m_IsOnGround = false;
			m_pRigidBody->activate(true);
		}
	}
	return m_IsOnGround;
}

Mob::IntentType Mob::GetIntent()
{
	return m_Intent;
}

bool Mob::OnGravityChange()
{
	if(Atom::OnGravityChange())
	{
		//grab an updated direction for gravity to orient ourselves against
		Ogre::Vector3 newGravityDir = Ogre::Vector3::ZERO;
		if(m_pCurrentTurf)
		{
			//grab a direction vector for gravity
			newGravityDir = m_pCurrentTurf->GetGravity().normalisedCopy();
			m_TargetStandingOrientation = OGRE2BT(Ogre::Vector3::NEGATIVE_UNIT_Y.getRotationTo(newGravityDir));

			//grab a quaternion representing a partially correct orientation
			//it's correct because it's upright relative to the current direction of gravity
			//it's incorrect because it doesn't retain the relative Y/Z angle of the mob
			/*Ogre::Quaternion targetIdentityOrientation = (m_pAtomRootSceneNode->getOrientation() * Ogre::Vector3::UNIT_Y).getRotationTo(newGravityDir);
			Ogre::Radian targetPitch = targetIdentityOrientation.getPitch();
			Ogre::Radian targetYaw = m_pAtomRootSceneNode->getOrientation().getYaw();
			Ogre::Radian targetRoll = m_pAtomRootSceneNode->getOrientation().getRoll();
			m_TargetStandingOrientation.setEuler(targetYaw.valueRadians(), targetPitch.valueRadians(), targetRoll.valueRadians());*/
		
			//let's grab the euler angles from it then via a matrix
			/*Ogre::Matrix3 targetIdentityOrientationMatrix;
			targetIdentityOrientation.ToRotationMatrix(targetIdentityOrientationMatrix);
			Ogre::Radian targetIdentityEulerX;
			Ogre::Radian targetIdentityEulerY;
			Ogre::Radian targetIdentityEulerZ;
			targetIdentityOrientationMatrix.ToEulerAnglesXYZ(targetIdentityEulerX, targetIdentityEulerY, targetIdentityEulerZ);
		
			//we want to reuse the current mob orientation for Z- and Y- axes, so lets get their euler angles as well (also via matrices)
			Ogre::Matrix3 currentOrientationMatrix;
			m_pAtomRootSceneNode->getOrientation().ToRotationMatrix(currentOrientationMatrix);
			Ogre::Radian currentEulerX;
			Ogre::Radian currentEulerY;
			Ogre::Radian currentEulerZ;
			currentOrientationMatrix.ToEulerAnglesXYZ(currentEulerX, currentEulerY, currentEulerZ);
		
			//turn the euler angles -> matrix -> ogre quaternion -> bullet quaternion
			Ogre::Matrix3 targetOrientationMatrix;
			targetOrientationMatrix.FromEulerAnglesXYZ(targetIdentityEulerX, currentEulerY, currentEulerZ);
			Ogre::Quaternion targetOrientation;
			targetOrientation.FromRotationMatrix(targetOrientationMatrix);
			m_TargetStandingOrientation = OGRE2BT(targetOrientation);
			m_TargetStandingOrientation = OGRE2BT(targetIdentityOrientation);*/
		}
	
		//check to see if we're facing the wrong way according to the new gravity
		//perpendicular checks won't work, so we'll have to directly compare the upwards vector
		bool reorient = false;
		if(newGravityDir.squaredLength())
		{
			Ogre::Vector3 upDir = m_pAtomRootSceneNode->getOrientation() * Ogre::Vector3::UNIT_Y;
			if(upDir != newGravityDir)
			{
				reorient = true;
				//std::cout << "	reorienting" << std::endl;
			}
			else
			{
				//std::cout << "	down vectors match" << std::endl;
			}
		}

		if(reorient)
		{
			m_tLeftUprightOrientation = 0;
			return true;
		}
	}
	return false;
}

void Mob::UpdateOrientation(float a_DeltaT)
{
	//orient physbody to make is upright relative to the direction of gravity
	if(m_pRigidBody && m_tLeftUprightOrientation < 1)
	{
		//just instantly snap to the right direction for now
		//float tValue = a_DeltaT * 2;
		//m_tLeftUprightOrientation += a_DeltaT;
		m_tLeftUprightOrientation = 1;
		/*if(m_tLeftUprightOrientation >= 1)
			tValue = 1;*/

		btTransform currentTransform = m_pRigidBody->getWorldTransform();

		//stand up over half a second
		btQuaternion currentOrientation = m_TargetStandingOrientation;//currentTransform.getRotation();
		//currentOrientation = currentOrientation.slerp(m_TargetStandingOrientation, tValue);

		//orient the rigidbody
		currentTransform.setRotation(currentOrientation);
		m_pRigidBody->setWorldTransform(currentTransform);
		
		//reset the model orientation
		//m_pAtomEntitySceneNode->setOrientation(Ogre::Quaternion::IDENTITY);

		//reset the camera orientation
		/*if(m_pPossessingClient && m_pPossessingClient->m_pCamera)
			m_pPossessingClient->m_pCamera->setOrientation( BT2OGRE(currentOrientation) );*/
	}
}

void Mob::SendClientMessage(std::string a_Message, int a_MsgType)
{
	if(m_pPossessingClient)
	{
		m_pPossessingClient->DisplayMessage(a_Message, (Client::MessageType)a_MsgType);
	}
}

bool Mob::AddInputModule(InputModule* a_pNewInputModule, bool a_Active)
{
	if(a_pNewInputModule)
	{
		if(a_Active)
		{
			m_ActiveInputModules.push_back(a_pNewInputModule);
		}
		else
		{
			m_InactiveInputModules.push_back(a_pNewInputModule);
		}
		return true;
	}
	return false;
}

bool Mob::DeactivateInputModule(InputModule* a_pInputModuleToDeactivate)
{
	if(a_pInputModuleToDeactivate)
	{
		for(auto it = m_ActiveInputModules.begin(); it != m_ActiveInputModules.end(); ++it)
		{
			if(*it == a_pInputModuleToDeactivate)
			{
				m_ActiveInputModules.erase(it);
				m_InactiveInputModules.push_back(a_pInputModuleToDeactivate);
				return true;
			}
		}
	}
	return false;
}

bool Mob::ReactivateInputModule(InputModule* a_pInputModuleToReactivate)
{
	if(a_pInputModuleToReactivate)
	{
		for(auto it = m_ActiveInputModules.begin(); it != m_ActiveInputModules.end(); ++it)
		{
			if(*it == a_pInputModuleToReactivate)
			{
				m_InactiveInputModules.erase(it);
				m_ActiveInputModules.push_back(a_pInputModuleToReactivate);
				return true;
			}
		}
	}
	return false;
}

bool Mob::ClearInputModule(InputModule* a_pInputModuleToClear, bool a_Active)
{
	if(a_pInputModuleToClear)
	{
		if(a_Active)
		{
			if(!ClearActiveInputModule(a_pInputModuleToClear))
			{
				return ClearInactiveInputModule(a_pInputModuleToClear);
			}
		}
		else
		{
			if(!ClearInactiveInputModule(a_pInputModuleToClear))
			{
				return ClearActiveInputModule(a_pInputModuleToClear);
			}
			else
			{
				return true;
			}
		}
	}
	return false;
}

bool Mob::ClearActiveInputModule(InputModule* a_pInputModuleToClear)
{
	for(auto it = m_ActiveInputModules.begin(); it != m_ActiveInputModules.end(); ++it)
	{
		if(*it == a_pInputModuleToClear)
		{
			m_ActiveInputModules.erase(it);
			return true;
		}
	}
	return false;
}

bool Mob::ClearInactiveInputModule(InputModule* a_pInputModuleToClear)
{
	for(auto it = m_InactiveInputModules.begin(); it != m_InactiveInputModules.end(); ++it)
	{
		if(*it == a_pInputModuleToClear)
		{
			m_InactiveInputModules.erase(it);
			return true;
		}
	}
	return false;
}
