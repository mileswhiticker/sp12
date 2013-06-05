#include "InputModule_GhostMob.h"
#include "Observer.hpp"
#include "AtomManager.hpp"
#include <OGRE\OgreSceneNode.h>

GhostMob::GhostMob(Mob* a_pOwnedMob, Client* a_pOwnedClient)
:	InputModule(a_pOwnedMob, a_pOwnedClient)
{
	//
}

void GhostMob::Update(float a_DeltaT)
{
	//
}

bool GhostMob::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return false;
}

bool GhostMob::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return false;
}

bool GhostMob::keyPressed( const OIS::KeyEvent &arg )
{
	return true;
}

bool GhostMob::keyReleased( const OIS::KeyEvent &arg )
{
	switch(arg.key)
	{
	case(OIS::KC_G):
		{
			//todo: bitshift spawn dir into the last argument
			Ogre::Vector3 spawnPos = Ogre::Vector3::ZERO;
			if(m_pOwnedMob)
				spawnPos = m_pOwnedMob->m_pAtomRootSceneNode->getPosition();
			Observer* pTestObserver = (Observer*)AtomManager::GetSingleton().CreateMob(Mob::OBSERVER, spawnPos, NULL, INSTANTIATE_IMMEDIATELY);
			pTestObserver->ConnectClient(m_pOwnedClient);

			pTestObserver->SetTargetPossessMob(m_pOwnedMob);
			m_pOwnedMob->DisconnectClient();
			break;
		}
	}
	return true;
}

bool GhostMob::mouseMoved( const OIS::MouseEvent &arg )
{
	return true;
}
