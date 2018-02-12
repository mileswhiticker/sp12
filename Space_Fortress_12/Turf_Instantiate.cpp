#include "Turf.hpp"

#include <OGRE\OgreEntity.h>
#include <OGRE\OgreSceneNode.h>
#include <OGRE\OgreSceneManager.h>

#include "OgreHelper.hpp"
#include "num2string.h"
#include <stdio.h>

void Turf::Instantiate(TurfType a_NewTurfType, Ogre::Entity* a_pCachedEntity)
{
	if(a_NewTurfType != m_MyTurfType)
	{
		//clear out old entity if necessary
		if( m_pAtomEntity && (a_NewTurfType == Turf::BUILDTURF || a_pCachedEntity) )
		{
			Ogre::SceneNode* pSceneNode = m_pAtomEntity->getParentSceneNode();
			if(pSceneNode)
			{
				pSceneNode->detachObject(m_pAtomEntity);
			}
			delete m_pAtomEntity;
			m_pAtomEntity = NULL;
			m_IsBuildPoint = true;
			//m_VirtualSelectMaterialName = "";
			//m_SelectMaterialName = "";
		}

		if(a_pCachedEntity)
		{
			m_pAtomEntity = a_pCachedEntity;
			m_pAtomEntitySceneNode->attachObject(m_pAtomEntity);
		}
		
		m_IsBuildPoint = false;

		//custom stuff for each turf type (entity material)
		switch(a_NewTurfType)
		{
		case(Turf::BUILDTURF):
			{
				if(!m_pAtomEntity)
				{
					//a single cuboid girder to cover this cell, uncomment to see buildturfs for debug purposes
					/*Ogre::SceneManager& sceneManager = GetSceneManager();
					m_pAtomEntity = sceneManager.createEntity("buildturf_" + num2string(m_AtomID), "cell_filling.mesh");
					m_pAtomEntity->setMaterialName(m_MaterialName);
					m_pAtomEntitySceneNode->attachObject(m_pAtomEntity);
					StopFlashingColour();*/
				}
				
				m_AtomFlags = RCD_BUILD_TARGET;

				break;
			}
		case(Turf::GIRDER_VIRTUAL):
			{
				if(!m_pAtomEntity)
				{
					//a single cuboid girder to cover this cell
					Ogre::SceneManager& sceneManager = GetSceneManager();
					m_pAtomEntity = sceneManager.createEntity("girder_" + num2string(m_AtomID), "girder.mesh");
					m_pAtomEntitySceneNode->attachObject(m_pAtomEntity);
					StopFlashingColour();
				}

				//m_AtomFlags &= RCD_BUILD_TARGET;
				//SetFlashingColour(Ogre::ColourValue::Green);
				m_MaterialName = "mat_highlight";
				m_SelectMaterialName = "girder_material_modulate2";
				m_pAtomEntity->setMaterialName(m_MaterialName);
				SetEntityVisible(false);
				m_UseRigidbodyPosition = false;

				break;
			}
		case(Turf::GIRDER):
			{
				if(!m_pAtomEntity)
				{
					//a single cuboid girder to cover this cell
					Ogre::SceneManager& sceneManager = GetSceneManager();
					m_pAtomEntity = sceneManager.createEntity("girder_" + num2string(m_AtomID), "girder.mesh");
					m_pAtomEntitySceneNode->attachObject(m_pAtomEntity);
					StopFlashingColour();
				}

				if(m_pAtomEntity)
				{
					m_MaterialName = "girder_material";
					m_SelectMaterialName = "girder_material_modulate";
					m_pAtomEntity->setMaterialName(m_MaterialName);
				}
				
				m_AtomFlags = RCD_CAN_DESTROY;

				//create buildpoints for girder
				/*
				UNKNOWNLEVEL = 0,
				//
				INNER,
				UNDER,
				CONDUIT1,
				CONDUIT2,
				OVER,
				OUTER,
				*/

				//std::list<Structure*> m_Structurepoints;

				break;
			}
		}
		
		m_MyTurfType = a_NewTurfType;
		//m_ContextAtomSubType = m_MyTurfType;
		std::cout << "";
	}
}

//unused, would any atom need this?
void Turf::InstantiateAtom()
{
	Instantiate(Turf::BUILDTURF);
}
