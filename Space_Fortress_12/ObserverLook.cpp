#include "ObserverLook.hpp"

#include "Mob.hpp"
#include "Client.hpp"
#include <OGRE\OgreCamera.h>

ObserverLook::ObserverLook(Mob* a_pOwnedMob)
:	InputModule(a_pOwnedMob)
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
	if(m_pOwnedMob->m_pPossessingClient && m_pOwnedMob->m_pPossessingClient->m_pCamera)
	{
		m_pOwnedMob->m_pPossessingClient->m_pCamera->yaw(Ogre::Radian(-float(arg.state.X.rel) / 1000.f));
		m_pOwnedMob->m_pPossessingClient->m_pCamera->pitch(Ogre::Radian(-float(arg.state.Y.rel) / 1000.f));
		return true;
	}
	return false;
}
