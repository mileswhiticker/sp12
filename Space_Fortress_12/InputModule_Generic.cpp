#include "InputModule_Generic.hpp"

#include "Mob.hpp"
#include "DebugDrawer.h"
#include "AtomManager.hpp"
#include <OGRE\OgreCamera.h>
#include <OGRE\OgreSceneNode.h>

Generic::Generic(Mob* a_pOwnedMob, Client* a_pOwnedClient)
:	InputModule(a_pOwnedMob, a_pOwnedClient)
{
	//
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
	case(OIS::KC_O):
		{
			DebugDrawer::getSingleton().setEnabled(!DebugDrawer::getSingleton().getEnabled());
			break;
		}
	case(OIS::KC_P):
		{
			if(!m_pOwnedMob)
				return false;

			Ogre::Vector3 outPos = m_pOwnedMob->m_pAtomSceneNode->getPosition();
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
