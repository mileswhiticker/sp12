#include "Observer.hpp"

#include "InputModule_ObserverBuild.hpp"
#include "InputModule_ObserverFlight.hpp"
#include "InputModule_Generic.hpp"
#include "InputModule_MouseLook.hpp"

#include <OGRE\OgreSceneNode.h>

Observer::Observer(Ogre::Vector3 a_StartPos, int a_Direction)
:	Mob(a_StartPos, a_Direction)
{
	m_MyMobType = OBSERVER;
	SetupInputModules();
}

void Observer::InstantiateAtom()
{
	//
}

void Observer::SetupInputModules()
{
	//debug: make all observers able to build
	m_InputModules.push_back(new Generic(this, NULL));
	m_InputModules.push_back(new MouseLook(this, NULL));
	m_InputModules.push_back(new ObserverFlight(this, NULL));
	m_InputModules.push_back(new ObserverBuild(this, NULL));
}
