#include "Observer.hpp"

#include "ObserverBuild.hpp"
#include "ObserverFlight.hpp"
#include "PlayerGeneric.hpp"
#include "ObserverLook.hpp"
#include "PossessMob.hpp"

#include <OGRE\OgreSceneNode.h>

Observer::Observer(Ogre::Vector3 a_StartPos, int a_Direction)
:	Mob(a_StartPos, a_Direction)
,	m_pPossessMob(NULL)
{
	m_MyMobType = OBSERVER;
	SetupInputModules();
	m_UsesGravity = false;
}

void Observer::InstantiateAtom()
{
	//
}

void Observer::SetupInputModules()
{
	//debug: make all observers able to build
	m_AllComponents.push_back(new PlayerGeneric(this, NULL));
	m_AllComponents.push_back(new ObserverLook(this, NULL));
	m_AllComponents.push_back(new ObserverFlight(this, NULL));
	m_AllComponents.push_back(new ObserverBuild(this, NULL));
}

void Observer::SetTargetPossessMob(Mob* a_pTargetPossessMob)
{
	if(!m_pPossessMob)
	{
		m_pPossessMob = new PossessMob(this, m_pPossessingClient);
		m_AllComponents.push_back(m_pPossessMob);
	}

	//todo
	m_pPossessMob->SetTargetMob(a_pTargetPossessMob);
}
