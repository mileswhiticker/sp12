#include "Observer.hpp"

#include "InputModule_ObserverBuild.hpp"
#include "InputModule_ObserverFlight.hpp"
#include "InputModule_Generic.hpp"

#include <OGRE\OgreSceneNode.h>

Observer::Observer()
{
	InstantiateAtom();
}

void Observer::InstantiateAtom()
{
	//debug: make all observers able to build
	m_InputModules.push_back(new Generic(this, NULL));
	m_InputModules.push_back(new ObserverFlight(this, NULL));
	m_InputModules.push_back(new ObserverBuild(this, NULL));
}
