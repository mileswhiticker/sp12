#include "Component.hpp"
#include "Atom.hpp"

Component::Component(Mob* a_pOwnedMob, Client* a_pOwnedClient)
:	m_pOwnedMob(a_pOwnedMob)
,	m_pOwnedClient(a_pOwnedClient)
,	m_pCurrentlyTargettedAtom(NULL)
{
	//
}

void Component::SetClient(Client* a_pClient)
{
	m_pOwnedClient = a_pClient;
	if(!m_pOwnedClient)
	{
		ClearSelectedAtom();
	}
}

bool Component::keyPressed( const OIS::KeyEvent &arg )
{
	return false;
}

bool Component::keyReleased( const OIS::KeyEvent &arg )
{
	return false;
}

bool Component::mouseMoved( const OIS::MouseEvent &arg )
{
	return false;
}

bool Component::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return false;
}

bool Component::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return false;
}

void Component::SelectNewAtom(Atom* a_pNewAtom)
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

void Component::ClearSelectedAtom()
{
	if(m_pCurrentlyTargettedAtom)
	{
		//std::cout << "clearing selected atom" << std::endl;
		m_pCurrentlyTargettedAtom->DeSelect(this);
		m_pCurrentlyTargettedAtom = NULL;
	}
}

void Component::ForceClearAtomIfSelected(Atom* a_pOtherAtom)
{
	if(a_pOtherAtom == m_pCurrentlyTargettedAtom)
	{
		ClearSelectedAtom();
	}
}
