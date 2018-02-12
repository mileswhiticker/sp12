#include "InputModule.hpp"
#include "Atom.hpp"
#include "Mob.hpp"

InputModule::InputModule(Mob* a_pOwnedMob)
:	m_pOwnedMob(a_pOwnedMob)
,	m_pCurrentlyTargettedAtom(NULL)
,	m_pLastInteractAtom(NULL)
{
	//
}

bool InputModule::TryInteract(Atom* a_pInteractAtom, bool a_DoInteract)
{
	return false;
}

void InputModule::TryInteract(std::vector<Atom*>& a_InteractAtoms)
{
	return;
}

float InputModule::GetInteractRange()
{
	return 0;
}

int InputModule::GetInteractFilterGroup()
{
	return 0;
}

int InputModule::GetInteractFilterMask()
{
	return 0;
}

int InputModule::GetInteractAtomType()
{
	return 0;
}

int InputModule::GetInteractAtomSubtype()
{
	return 0;
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

void InputModule::SetMobOwner(Mob* a_pMob)
{
	m_pOwnedMob = a_pMob;
}








//todo: delete these?
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
