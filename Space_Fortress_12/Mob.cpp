#include "Mob.hpp"

#include "Component.hpp"
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
{
	m_MyAtomType = Atom::MOB;
	//btQuaternion(btVector3(0,1,0), 0);
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

bool Mob::ConnectClient(Client* a_pNewClient)
{
	if(!m_pPossessingClient)
	{
		m_pPossessingClient = a_pNewClient;
		m_pPossessingClient->m_pPossessedMob = this;
		m_pPossessingClient->ResetCamera();

		RegisterKeyListener(this);
		RegisterMouseListener(this);
		
		for(auto it = m_AllComponents.begin(); it != m_AllComponents.end(); ++it)
		{
			(*it)->SetClient(a_pNewClient);
		}

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

		for(auto it = m_AllComponents.begin(); it != m_AllComponents.end(); ++it)
		{
			(*it)->SetClient(NULL);
		}

		return true;
	}
	return false;
}

bool Mob::keyPressed( const OIS::KeyEvent &arg )
{
	for(auto it = m_AllComponents.begin(); it != m_AllComponents.end(); ++it)
	{
		(*it)->keyPressed(arg);
	}

	return false;
}

bool Mob::keyReleased( const OIS::KeyEvent &arg )
{
	for(auto it = m_AllComponents.begin(); it != m_AllComponents.end(); ++it)
	{
		(*it)->keyReleased(arg);
	}

	return false;
}

bool Mob::mouseMoved( const OIS::MouseEvent &arg )
{
	for(auto it = m_AllComponents.begin(); it != m_AllComponents.end(); ++it)
	{
		(*it)->mouseMoved(arg);
	}

	return false;
}

bool Mob::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	for(auto it = m_AllComponents.begin(); it != m_AllComponents.end(); ++it)
	{
		(*it)->mousePressed(arg, id);
	}

	return false;
}

bool Mob::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	for(auto it = m_AllComponents.begin(); it != m_AllComponents.end(); ++it)
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

void Mob::AddObjectToInventory(Object* a_pObject)
{
	//if this object is held by another mob, it will be forcibly removed from that one
	//if we already hold this object, it'll do nothing
	a_pObject->AddToMobInventory(this);

	//only add it to our inventory if we don't hold it already
	const int atomUID = a_pObject->GetAtomUID();
	if(m_Contents.count(atomUID) == 0)
	{
		m_Contents[atomUID] = a_pObject;
	}
}

void Mob::RemoveObjectFromInventory(Object* a_pObject)
{
	const int atomUID = a_pObject->GetAtomUID();
	if(m_Contents.count(atomUID) == 1)
	{
		m_Contents.erase(atomUID);
	}
}

void Mob::SendClientMessage(std::string a_Message, int a_MsgType)
{
	if(m_pPossessingClient)
	{
		m_pPossessingClient->DisplayMessage(a_Message, (Client::MessageType)a_MsgType);
	}
}
