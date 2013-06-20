#include "InputModule_PossessMob.hpp"
#include "Mob.hpp"
#include "Client.hpp"

#include <OGRE\OgreCamera.h>
#include <OGRE\OgreSceneNode.h>
#include <LinearMath\btTransform.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>
#include <cegui\elements\CEGUIGUISheet.h>

#include "BtOgreHelper.hpp"

PossessMob::PossessMob(Mob* a_pOwnedMob, Client* a_pOwnedClient)
:	InputModule(a_pOwnedMob, a_pOwnedClient)
,	m_pTargetPossessMob(NULL)
{
	//
}

void PossessMob::Update(float a_DeltaT)
{
	//
}

bool PossessMob::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return false;
}

bool PossessMob::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return false;
}

bool PossessMob::keyPressed( const OIS::KeyEvent &arg )
{
	return true;
}

bool PossessMob::keyReleased( const OIS::KeyEvent &arg )
{
	switch(arg.key)
	{
	case(OIS::KC_G):
		{
			if(m_pTargetPossessMob)
			{
				//evict the clients client currently in there, if there is one
				m_pTargetPossessMob->DisconnectClient();
				Client* pOwnClient = m_pOwnedClient;
				if(m_pOwnedMob)
				{
					m_pOwnedMob->DisconnectClient();
				}
				m_pTargetPossessMob->ConnectClient(pOwnClient);
				pOwnClient->m_pTopInfoBar->setText("Player mode");

				//todo: delete this->m_pOwnedMob (?)
				//
			}
			break;
		}
	case(OIS::KC_H):
		{
			if(m_pTargetPossessMob)
			{
				btTransform newTransform = m_pTargetPossessMob->m_pRigidBody->getWorldTransform();
				Ogre::Vector3 newPos = m_pOwnedMob->m_pAtomRootSceneNode->_getDerivedPosition();
				newTransform.setOrigin( OGRE2BT(newPos) );
				m_pTargetPossessMob->m_pRigidBody->setWorldTransform(newTransform);
				m_pTargetPossessMob->ResetEnvironment();
			}

			return true;
		}
	}
	return true;
}

bool PossessMob::mouseMoved( const OIS::MouseEvent &arg )
{
	return true;
}

void PossessMob::SetTargetMob(Mob* a_pTargetPossessMob)
{
	//should the old mob be notified?
	m_pTargetPossessMob = a_pTargetPossessMob;
}
