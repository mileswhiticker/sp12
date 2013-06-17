#include "InputModule.hpp"
#include "Atom.hpp"

InputModule::InputModule(Mob* a_pOwnedMob, Client* a_pOwnedClient)
:	m_pOwnedMob(a_pOwnedMob)
,	m_pOwnedClient(a_pOwnedClient)
,	m_pCurrentlyTargettedAtom(NULL)
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

void InputModule::SelectNewAtom(Atom* a_pNewAtom)
{
	if(a_pNewAtom)
	{
		if(a_pNewAtom != m_pCurrentlyTargettedAtom)
		{
			ClearSelectedAtom();
			m_pCurrentlyTargettedAtom = a_pNewAtom;
			m_pCurrentlyTargettedAtom->Select(this);
			//std::cout << "new atom targetted" << std::endl;
		}
	}
	else
	{
		ClearSelectedAtom();
	}
}

void InputModule::ClearSelectedAtom()
{
	if(m_pCurrentlyTargettedAtom)
	{
		//std::cout << "clearing selected atom" << std::endl;
		m_pCurrentlyTargettedAtom->DeSelect(this);
		m_pCurrentlyTargettedAtom = NULL;
	}
}

void InputModule::ForceClearAtomIfSelected(Atom* a_pOtherAtom)
{
	if(a_pOtherAtom == m_pCurrentlyTargettedAtom)
	{
		ClearSelectedAtom();
	}
}
