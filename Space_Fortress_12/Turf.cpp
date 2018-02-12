#include "Turf.hpp"
#include "Events.hpp"
#include "Buildpoint.hpp"
//#include "MapCell.hpp"
#include "Structure.hpp"
#include "Mob.hpp"

#include "AtomManager.hpp"

#include <OGRE\OgreSceneNode.h>
#include <OGRE\OgreEntity.h>

#include <BulletCollision\CollisionShapes\btBoxShape.h>
#include <BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>
#include <LinearMath\btDefaultMotionState.h>
#include <LinearMath\btVector3.h>

#include "BtOgreHelper.hpp"
#include "OgreHelper.hpp"
#include "BulletHelper.hpp"
#include "MapHelper.hpp"
#include "Turf.hpp"

#include "CollisionDefines.h"
#include "Direction.h"
#include "num2string.h"

Turf::Turf(Ogre::Vector3 a_Location)
:	Atom(a_Location)
//,	Context(this, this)		//safe in this instance, not good general practice
//,	m_pTurfStructure(0)
,	m_MyTurfType(TURF_UNKNOWN)
,	m_IsBuildPoint(true)
,	m_CoordsString("")//GetCoordsString(a_Location))
{
	m_MyAtomType = Atom::TURF;
	m_pAtomRootSceneNode->setPosition(a_Location);
	m_UpdateTurfLoc = false;
	
	//initialise context stuff here, instead of in Context::Context()
	/*m_ContextAtomType = m_MyAtomType;
	m_ContextAtomSubType = m_MyTurfType;
	m_MyContextType = Context::BUILD;*/

	/*for(int curDir = 1; curDir <= 32; curDir *= 2)
	{
		for(int curPos = Buildpoint::INNER_PLATING_OUT; curPos <= Buildpoint::POS_MAXVAL; curPos += 1)
		{
			std::stringstream out;
			out << curDir << curPos;
			m_Buildpoints.insert( std::pair<std::string, Buildpoint*>(out.str(), new Buildpoint(*this, curPos, curDir, false) ) );
		}
	}*/

	//create an invisible cube for us to "interact" with
	Ogre::SceneManager& sceneManager = GetSceneManager();
	//std::cout << "instantiating OverlayPlating with direction " << m_Direction << std::endl;
	
	//create entity
	/*m_pAtomEntity = sceneManager.createEntity("turf" + num2string(m_AtomID), "cell_filling.mesh");
	m_pAtomEntitySceneNode->attachObject(m_pAtomEntity);
	StopFlashingColour();*/

	//set up the directional offsets
	Ogre::Vector3 offsetPos(0, 0, 0);
	Ogre::Vector3 lookatPos(0, 0, 0);
	btVector3 halfExtents(0.5f, 0.5f, 0.5f);
	//
	m_pAtomEntitySceneNode->setPosition(offsetPos);
	m_pAtomEntitySceneNode->lookAt(lookatPos, Ogre::Node::TS_LOCAL);
	//m_pAtomEntitySceneNode->yaw(Ogre::Degree(90));
	
	//create physics body and initialise to starting position
	m_pCollisionShape = new btBoxShape(halfExtents);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), OGRE2BT(m_pAtomEntitySceneNode->_getDerivedPosition())));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, m_pCollisionShape, btVector3(0,0,0));
	m_pRigidBody = new btRigidBody(groundRigidBodyCI);
	m_pRigidBody->setUserPointer(this);

	//add new rigid body to world
	btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();
	//SetEntityVisible(false);
	//m_pAtomEntity->setMaterialName("cell_highlight_material");
	dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_TURF, RAYCAST_BUILD);

	//InitCollisionShapeDebugDraw(Ogre::ColourValue::Red);
	//
}

Turf::~Turf()
{
	//eject all contents
	while(m_AtomContents.size() > 0)
	{
		(*m_AtomContents.begin())->ForceEjectFromTurf();
		m_AtomContents.erase(m_AtomContents.begin());
	}
}
