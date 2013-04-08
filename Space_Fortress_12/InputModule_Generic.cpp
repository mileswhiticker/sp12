#include "InputModule_Generic.hpp"

#include <OGRE\OgreCamera.h>
#include <OGRE\OgreSceneNode.h>
#include <OGRE\OgreLight.h>

#include "OgreHelper.hpp"
#include "Mob.hpp"
#include "DebugDrawer.h"
#include "AtomManager.hpp"
#include "Client.hpp"

Generic::Generic(Mob* a_pOwnedMob, Client* a_pOwnedClient)
:	InputModule(a_pOwnedMob, a_pOwnedClient)
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

void Generic::Update(float a_DeltaT)
{
	//
}

bool Generic::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return false;
}

bool Generic::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return false;
}

bool Generic::keyPressed( const OIS::KeyEvent &arg )
{
	return true;
}

bool Generic::keyReleased( const OIS::KeyEvent &arg )
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
			break;
		}
	case(OIS::KC_P):
		{
			if(!m_pOwnedMob)
				return false;

			Ogre::Vector3 outPos = m_pOwnedMob->m_pAtomEntitySceneNode->getPosition();
			std::cout << outPos.x << ", " << outPos.y << ", " << outPos.z << std::endl;
			break;
		}
	case(OIS::KC_F):
		{
			AtomManager::GetSingleton().ToggleCellFlashing();
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

bool Generic::mouseMoved( const OIS::MouseEvent &arg )
{
	return true;
}
