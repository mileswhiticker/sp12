#include "InputModule.hpp"

InputModule::InputModule(Mob* a_pOwnedMob, Client* a_pOwnedClient)
:	m_pOwnedMob(a_pOwnedMob)
,	m_pOwnedClient(a_pOwnedClient)
{
	//
}

void InputModule::SetClient(Client* a_pClient)
{
	m_pOwnedClient = a_pClient;
}

bool InputModule::keyPressed( const OIS::KeyEvent &arg )
{
	return false;
}

bool InputModule::keyReleased( const OIS::KeyEvent &arg )
{
	return false;
}

bool InputModule::mouseMoved( const OIS::MouseEvent &arg )
{
	return false;
}

bool InputModule::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return false;
}

bool InputModule::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return false;
}
