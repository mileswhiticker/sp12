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
