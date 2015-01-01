#include "Observer.hpp"

#include "RCD.hpp"
//
#include "ObserverFlight.hpp"
#include "PlayerGeneric.hpp"
#include "ObserverLook.hpp"
#include "PossessMob.hpp"
#include "Inventory.hpp"

#include <OGRE\OgreSceneNode.h>

Observer::Observer(Ogre::Vector3 a_StartPos, int a_Direction)
:	Mob(a_StartPos, a_Direction)
,	m_pPossessMob(NULL)
,	m_pMyRCD(NULL)
{
	m_MyMobType = OBSERVER;
	m_UsesGravity = false;
	
	m_ActiveInputModules.push_back(new PlayerGeneric(this));
	m_ActiveInputModules.push_back(new ObserverLook(this));
	m_ActiveInputModules.push_back(new ObserverFlight(this));
	m_ActiveInputModules.push_back(new Inventory(this));
	//AddAtomToContents(new RCD(Ogre::Vector3::ZERO));

	m_pMyRCD = new RCD(Ogre::Vector3(0,0,0));
	m_pCurrentRaycastingInputModule = m_pMyRCD->GetEquipInputModule();
	m_ActiveInputModules.push_back(m_pCurrentRaycastingInputModule);
	m_pCurrentRaycastingInputModule->SetMobOwner(this);
}

void Observer::InstantiateAtom()
{
	//
}

void Observer::SetTargetPossessMob(Mob* a_pTargetPossessMob)
{
	if(!m_pPossessMob)
	{
		m_pPossessMob = new PossessMob(this);
		m_ActiveInputModules.push_back(m_pPossessMob);
	}

	//todo
	m_pPossessMob->SetTargetMob(a_pTargetPossessMob);
}
