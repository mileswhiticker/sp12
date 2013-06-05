#include "Observer.hpp"

#include "InputModule_ObserverBuild.hpp"
#include "InputModule_ObserverFlight.hpp"
#include "InputModule_Generic.hpp"
#include "InputModule_ObserverLook.hpp"
#include "InputModule_PossessMob.hpp"

#include <OGRE\OgreSceneNode.h>

Observer::Observer(Ogre::Vector3 a_StartPos, int a_Direction)
:	Mob(a_StartPos, a_Direction)
,	m_pPossessMobInputModule(NULL)
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
	m_InputModules.push_back(new Generic(this, NULL));
	m_InputModules.push_back(new ObserverLook(this, NULL));
	m_InputModules.push_back(new ObserverFlight(this, NULL));
	m_InputModules.push_back(new ObserverBuild(this, NULL));
}

void Observer::SetTargetPossessMob(Mob* a_pTargetPossessMob)
{
	if(!m_pPossessMobInputModule)
	{
		m_pPossessMobInputModule = new PossessMob(this, m_pPossessingClient);
		m_InputModules.push_back(m_pPossessMobInputModule);
	}

	//todo
	m_pPossessMobInputModule->SetTargetMob(a_pTargetPossessMob);
}
