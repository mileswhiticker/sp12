#include "MapCell.hpp"
#include "GravitySource.hpp"
#include "Atom.hpp"
#include "MapHelper.hpp"

#include "LinearMath\btVector3.h"

MapCell::MapCell(Ogre::Vector3 a_Position)
:	m_pMyCellTurf(NULL)
//,	m_CombinedGravity(Ogre::Vector3::ZERO)
/*,	m_pAdjNorth(0)
,	m_pAdjSouth(0)
,	m_pAdjEast(0)
,	m_pAdjWest(0)
,	m_pAdjUp(0)
,	m_pAdjDown(0)*/
,	m_Position(a_Position)
{
	// Create build raycast targets
	for(int curDir = 1; curDir <= 32; curDir *= 2)
	{
		// Which side of the cell is it?
		Ogre::Vector3 dirVector = GetUnitVectorFromDir(curDir);

		// Setup the transforms
		Ogre::Vector3 offsetPos = dirVector * 0.505;
		Ogre::Vector3 lookatPos = 2 * dirVector;

		btVector3 halfExtents(0.5f, 0.5f, 0.5f);
		halfExtents.setX(offsetPos.x * 0.005f);
		halfExtents.setY(offsetPos.y * 0.005f);
		halfExtents.setZ(offsetPos.z * 0.005f);
	}
	/*
	m_pAtomEntitySceneNode->setPosition(offsetPos);
	m_pAtomEntitySceneNode->lookAt(lookatPos, Ogre::Node::TS_LOCAL);
	m_pAtomEntitySceneNode->yaw(Ogre::Degree(90));
	
	//create physics body and initialise to starting position
	m_pCollisionShape = new btBoxShape(halfExtents);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), OGRE2BT(m_pAtomEntitySceneNode->_getDerivedPosition())));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, m_pCollisionShape, btVector3(0,0,0));
	m_pRigidBody = new btRigidBody(groundRigidBodyCI);
	m_pRigidBody->setUserPointer(this);

	//add new rigid body to world
	btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();
	if(m_IsBuildPoint)
	{
		SetEntityVisible(false);
		m_pAtomEntity->setMaterialName("cell_highlight_material");
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_BUILDPOINT, RAYCAST_BUILD);

		//todo: is this working?
		//m_pRigidBody->setCollisionFlags(m_pRigidBody->CF_NO_CONTACT_RESPONSE);
	}
	else
	{
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_STRUCTURE, RAYCAST_BUILD|COLLISION_OBJ|COLLISION_MOB);
	}
	*/
}

Ogre::Vector3 MapCell::GetGravity()
{
	Ogre::Vector3 out = Ogre::Vector3::ZERO;
	for(auto it = m_GravityAffectors.begin(); it != m_GravityAffectors.end(); ++it)
	{
		out += it->first->GetGravity() * it->second;
	}
	return out;
}

void MapCell::AddGravityAffector(GravitySource* a_pGravSource, float a_DistScalar)
{
	m_GravityAffectors.push_back( std::pair<GravitySource*,float>(a_pGravSource, a_DistScalar) );
	OnGravityChange();
}

void MapCell::RemoveGravityAffector(GravitySource* a_pGravSource)
{
	for(auto it = m_GravityAffectors.begin(); it != m_GravityAffectors.end(); ++it)
	{
		if(it->first == a_pGravSource)
		{
			m_GravityAffectors.erase(it);
			break;
		}
	}
	OnGravityChange();
}

void MapCell::AtomEnterCell(Atom* a_pEnteringAtom)
{
	m_AtomsInCell.push_back(a_pEnteringAtom);
	a_pEnteringAtom->OnGravityChange();
}

void MapCell::AtomLeaveCell(Atom* a_pLeavingAtom)
{
	for(auto it = m_AtomsInCell.begin(); it != m_AtomsInCell.end(); ++it)
	{
		if(*it == a_pLeavingAtom)
		{
			m_AtomsInCell.erase(it);
			break;
		}
	}
}

void MapCell::OnGravityChange()
{
	for(auto it = m_AtomsInCell.begin(); it != m_AtomsInCell.end(); ++it)
	{
		(*it)->OnGravityChange();
	}
}
