#include "MobGhost.hpp"
#include "Observer.hpp"
#include "AtomManager.hpp"
#include "Client.hpp"

#include <cegui\elements\CEGUIGUISheet.h>
#include <OGRE\OgreSceneNode.h>

MobGhost::MobGhost(Mob* a_pOwnedMob, Client* a_pOwnedClient)
:	Component(a_pOwnedMob, a_pOwnedClient)
{
	//
}

void MobGhost::Update(float a_DeltaT)
{
	//
}

bool MobGhost::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return false;
}

bool MobGhost::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return false;
}

bool MobGhost::keyPressed( const OIS::KeyEvent &arg )
{
	return true;
}

bool MobGhost::keyReleased( const OIS::KeyEvent &arg )
{
	switch(arg.key)
	{
	case(OIS::KC_G):
		{
			//todo: bitshift spawn dir into the last argument
			Ogre::Vector3 spawnPos = Ogre::Vector3::ZERO;
			m_pOwnedClient->m_pTopInfoBar->setText("Observer mode");
			if(m_pOwnedMob)
				spawnPos = m_pOwnedMob->m_pAtomRootSceneNode->getPosition();
			Observer* pTestObserver = (Observer*)AtomManager::GetSingleton().CreateMob(Mob::OBSERVER, spawnPos, INSTANTIATE_IMMEDIATELY);
			pTestObserver->ConnectClient(m_pOwnedClient);

			pTestObserver->SetTargetPossessMob(m_pOwnedMob);
			m_pOwnedMob->DisconnectClient();
			break;
		}
	}
	return true;
}

bool MobGhost::mouseMoved( const OIS::MouseEvent &arg )
{
	return true;
}
