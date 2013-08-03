#include "PlayerGeneric.hpp"

#include <OGRE\OgreCamera.h>
#include <OGRE\OgreSceneNode.h>
#include <OGRE\OgreLight.h>

#include "OgreHelper.hpp"
#include "Mob.hpp"
#include "DebugDrawer.h"
#include "AtomManager.hpp"
#include "Client.hpp"
#include "EffectManager.hpp"
#include "Object.hpp"

PlayerGeneric::PlayerGeneric(Mob* a_pOwnedMob, Client* a_pOwnedClient)
:	Component(a_pOwnedMob, a_pOwnedClient)
{
	//point light for testing
	Ogre::SceneManager& sceneMgr = GetSceneManager();
	m_pPointLight = sceneMgr.createLight();
	m_pPointLight->setType(Ogre::Light::LT_POINT);
	m_pPointLight->setPosition(10,5,0);
	m_pPointLight->setDiffuseColour(1.0, 1.0, 1.0);
	//m_pPointLight->setSpecularColour(0.0, 0.0, 1.0);
	m_pPointLight->setAttenuation(25, 0, 1, 0);
}

void PlayerGeneric::Update(float a_DeltaT)
{
	//
}

bool PlayerGeneric::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return false;
}

bool PlayerGeneric::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return false;
}

bool PlayerGeneric::keyPressed( const OIS::KeyEvent &arg )
{
	return true;
}

bool PlayerGeneric::keyReleased( const OIS::KeyEvent &arg )
{
	switch(arg.key)
	{
	case(OIS::KC_L):
		{
			if(m_pOwnedClient)
				m_pPointLight->setPosition(m_pOwnedClient->m_pCamera->getDerivedPosition());
			break;
		}
	case(OIS::KC_O):
		{
			DebugDrawer::getSingleton().setEnabled(!DebugDrawer::getSingleton().getEnabled());
			EffectManager::GetSingleton().ForceRebuild();
			break;
		}
	case(OIS::KC_N):
		{
			AtomManager::GetSingleton().CreateObject(Object::BOX, m_pOwnedMob->m_pAtomRootSceneNode->_getDerivedPosition());
			break;
		}
	case(OIS::KC_P):
		{
			if(!m_pOwnedMob)
				return false;

			Ogre::Vector3 outPos = m_pOwnedMob->m_pAtomRootSceneNode->getPosition();
			std::cout << outPos.x << ", " << outPos.y << ", " << outPos.z << std::endl;
			break;
		}
	case(OIS::KC_F):
		{
			//AtomManager::GetSingleton().ToggleCellFlashing();
			break;
		}
	case(OIS::KC_ESCAPE):
		{
			exit(0);
			break;
		}
	}
	return true;
}

bool PlayerGeneric::mouseMoved( const OIS::MouseEvent &arg )
{
	return true;
}
