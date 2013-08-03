#include "ObserverLook.hpp"

#include "Client.hpp"
#include <OGRE\OgreCamera.h>

ObserverLook::ObserverLook(Mob* a_pOwnedMob, Client* a_pOwnedClient)
:	Component(a_pOwnedMob, a_pOwnedClient)
,	m_LookAngle(Ogre::Vector3(0,0,0))
,	m_LookOrientation(Ogre::Quaternion::IDENTITY)
{
	//
}

void ObserverLook::Update(float a_DeltaT)
{
	//
}

bool ObserverLook::mouseMoved( const OIS::MouseEvent &arg )
{
	if(m_pOwnedClient && m_pOwnedClient->m_pCamera)
	{
		m_pOwnedClient->m_pCamera->yaw(Ogre::Radian(-float(arg.state.X.rel) / 1000.f));
		m_pOwnedClient->m_pCamera->pitch(Ogre::Radian(-float(arg.state.Y.rel) / 1000.f));
		return true;
	}
	return false;
}
