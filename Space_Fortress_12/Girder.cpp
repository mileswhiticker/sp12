#include "Girder.hpp"

#include <OGRE\OgreSceneManager.h>
#include <OGRE\OgreSceneNode.h>
#include <OGRE\OgreEntity.h>
#include <OGRE\OgreSubEntity.h>

#include <BulletCollision\CollisionShapes\btBoxShape.h>
#include <BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>
#include <LinearMath\btDefaultMotionState.h>

#include "AtomManager.hpp"
#include "MapSuite.hpp"

#include "BtOgreHelper.hpp"
#include "OgreHelper.hpp"
#include "BulletHelper.hpp"

#include "MapCell.hpp"
#include "num2string.h"
#include "UID.hpp"
#include "Direction.h"
#include "DebugDrawer.h"
#include "CollisionDefines.h"

Girder::Girder(MapCell* a_pSourceMapCell)
:	Structure(Ogre::Vector3::ZERO, 0)
,	Turf()
,	m_PlateOverlayDirs(0)
,	m_PlateUnderlays(0)
,	m_pSourceMapCell(a_pSourceMapCell)
{
	m_MyStructureType = Structure::GIRDER;
	m_pTurfStructure = this;
	//
	if(m_pSourceMapCell)
	{
		m_pAtomEntitySceneNode->setPosition(m_pSourceMapCell->m_Position);
		a_pSourceMapCell->m_pMyCellTurf = this;
	}
}

Girder::~Girder()
{
	//if we're not a build point, turn us into one (checks inside the func)
	DestroyToBuildPoint();
}

void Girder::InstantiateStructure(bool a_IsBuildPoint)
{
	m_IsBuildPoint = a_IsBuildPoint;

	//a single cuboid girder to cover this cell
	Ogre::SceneManager& sceneManager = GetSceneManager();
	m_pAtomEntity = sceneManager.createEntity(num2string(NewUID()) + " girder", "girder.mesh");
	m_pAtomEntitySceneNode->attachObject(m_pAtomEntity);
	StopFlashingColour();
	
	//create physics collider to intercept raycasts
	btBoxShape* m_pCollisionShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), OGRE2BT(m_pAtomEntitySceneNode->getPosition())));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, m_pCollisionShape, btVector3(0,0,0));
	m_pRigidBody = new btRigidBody(groundRigidBodyCI);
	m_pRigidBody->setUserPointer(this);
	
	//todo: is this working?
	m_pRigidBody->setCollisionFlags(m_pRigidBody->CF_NO_CONTACT_RESPONSE);

	//add new rigid body to world
	btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();
	if(m_IsBuildPoint)
	{
		//std::cout << "Instantiate() called on structure build point" << std::endl;
		SetEntityVisible(false);
		m_pAtomEntity->setMaterialName("cell_highlight_material");
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_BUILDPOINT, COLLISION_BUILDRAYCAST);
	}
	else
	{
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_STRUCTURE, COLLISION_BUILDRAYCAST);

		//create structure buildpoints
		for(int curDir = 1; curDir <= 32; curDir *= 2)
		{
			//std::cout << "direction: " << curDir << std::endl;
			Structure* pUnusedBuildPoint = AtomManager::GetSingleton().CreateStructure(Structure::OVERLAYPLATING, m_pSourceMapCell, NULL, curDir|BUILD_POINT|INSTANTIATE_IMMEDIATELY);
			m_InvisibleBuildPoints.push_back(pUnusedBuildPoint);

			pUnusedBuildPoint = AtomManager::GetSingleton().CreateStructure(Structure::UNDERLAYPLATING, m_pSourceMapCell, NULL, curDir|BUILD_POINT|INSTANTIATE_IMMEDIATELY);
			m_InvisibleBuildPoints.push_back(pUnusedBuildPoint);

			pUnusedBuildPoint = AtomManager::GetSingleton().CreateStructure(Structure::GRAVPLATES, m_pSourceMapCell, NULL, curDir|BUILD_POINT|INSTANTIATE_IMMEDIATELY);
			m_InvisibleBuildPoints.push_back(pUnusedBuildPoint);
		}
	}
}

void Girder::AddFreefloatingObj(std::string a_TypeTag)
{
	if(m_pAtomEntitySceneNode)
	{
		AtomManager::GetSingleton().CreateAtom(Atom::OBJECT, m_pAtomEntitySceneNode->getPosition());
	}
	else
	{
		std::cout << "WARNING: Trying to add object of type '" << a_TypeTag << "' to empty (uninitialised?) cell." << std::endl;
	}
}

void Girder::CreateFromBuildPoint()
{
	if(m_IsBuildPoint)
	{
		//std::cout << "creating girder from build point" << std::endl;

		//update the collision flags
		btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();
		dynamicsWorld.removeRigidBody(m_pRigidBody);
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_STRUCTURE, COLLISION_BUILDRAYCAST);

		//create plating buildpoints
		for(int curDir = 1; curDir <= 32; curDir *= 2)
		{
			//std::cout << "direction: " << curDir << std::endl;
			Structure* pUnusedBuildPoint = AtomManager::GetSingleton().CreateStructure(Structure::OVERLAYPLATING, m_pSourceMapCell, NULL, curDir|BUILD_POINT|INSTANTIATE_IMMEDIATELY);
			m_InvisibleBuildPoints.push_back(pUnusedBuildPoint);

			pUnusedBuildPoint = AtomManager::GetSingleton().CreateStructure(Structure::UNDERLAYPLATING, m_pSourceMapCell, NULL, curDir|BUILD_POINT|INSTANTIATE_IMMEDIATELY);
			m_InvisibleBuildPoints.push_back(pUnusedBuildPoint);
		}

		//miscellaneous
		m_pAtomEntity->setMaterialName("girder_material");
		SetEntityVisible(true);
		m_IsBuildPoint = false;
		
		//create girder buildpoints in adjacent cells
		Ogre::Vector3 pos = m_pAtomEntitySceneNode->getPosition();
		MapSuite::GetInstance().CreateAdjacentGirderBuildpoints(m_pSourceMapCell);
	}
}

void Girder::DestroyToBuildPoint()
{
	if(!m_IsBuildPoint)
	{
		//std::cout << "destroying girder to build point" << std::endl;

		//reset the collision flags for build raycasting
		btDiscreteDynamicsWorld& dynamicsWorld = GetDynamicsWorld();
		dynamicsWorld.removeRigidBody(m_pRigidBody);
		dynamicsWorld.addRigidBody(m_pRigidBody, COLLISION_BUILDPOINT, COLLISION_BUILDRAYCAST);

		//reset the material
		m_pAtomEntity->setMaterialName("cell_highlight_material");
		
		//delete all mounted structures
		for(auto it = m_MountedStructures.begin(); it != m_MountedStructures.end(); ++it)
		{
			AtomManager::GetSingleton().DeleteStructure(*it);
		}
		m_MountedStructures.erase(m_MountedStructures.begin(), m_MountedStructures.end());
		
		//delete all structure buildpoints
		for(auto it = m_InvisibleBuildPoints.begin(); it != m_InvisibleBuildPoints.end(); ++it)
		{
			AtomManager::GetSingleton().DeleteStructure(*it);
		}
		m_InvisibleBuildPoints.erase(m_InvisibleBuildPoints.begin(), m_InvisibleBuildPoints.end());
		
		//done
		SetEntityVisible(false);
		m_IsBuildPoint = true;

		MapSuite::GetInstance().ClearDependantAdjacentGirderBuildpoints(m_pSourceMapCell);
	}
}

void Girder::CreateBuildpointInDir(Structure::StructureType a_BuildPointType, int a_Dir)
{
	bool success = false;
	auto it_end = m_InvisibleBuildPoints.end();
	for(auto it = m_InvisibleBuildPoints.begin(); it != m_InvisibleBuildPoints.end(); it)
	{
		//create the first non-buildpoint structure that matches, and delete the rest (if there are any others, there shouldn't be so this is just in case)
		if((*it)->GetStructureType() == a_BuildPointType && (*it)->GetDirection() == a_Dir)
		{
			if(!success)
			{
				//if it's a build point, create it
				if((*it)->IsBuildPoint())
				{
					(*it)->CreateFromBuildPoint();
				}

				//now move it over to the list of 'created' build points
				m_MountedStructures.push_back(*it);
				it = m_InvisibleBuildPoints.erase(it);
				success = true;
			}
			else
			{
				//delete it, because it must be a duplicate
				AtomManager::GetSingleton().DeleteStructure(*it);
				it = m_InvisibleBuildPoints.erase(it);
			}
		}
		else
		{
			++it;
		}
	}
}

void Girder::Select(ObserverBuild* a_pSelectingObserver)
{
	Atom::Select(a_pSelectingObserver);
	if(m_pAtomEntity)
	{
		if(m_IsBuildPoint)
			m_pAtomEntity->setMaterialName("cell_highlight_material");
		else
			m_pAtomEntity->setMaterialName("girder_material_modulate");
	}
}

void Girder::DeSelect(ObserverBuild* a_pSelectingObserver)
{
	Atom::DeSelect(a_pSelectingObserver);
	if(m_pAtomEntity)
	{
		m_pAtomEntity->setMaterialName("girder_material");
	}
}
