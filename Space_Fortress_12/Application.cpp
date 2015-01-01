#include "Application.hpp"
#include "GameManager.hpp"
#include "GlueScreen.hpp"
#include "EffectManager.hpp"

#include <OGRE\OgreRenderWindow.h>
#include "DebugDrawer.h"

#include <cegui\CEGUISystem.h>

Application& Application::GetSingleton()
{
	static Application instance;
	return instance;
}

Application::Application()
:	m_GameManager(GameManager::GetSingleton())
,	m_ShutdownNextUpdate(false)
	//
,	m_pRoot(NULL)
,   m_pWindow(NULL)
,	m_pSceneManager(NULL)
	//
,	m_pInputManager(NULL)
,	m_pKeyboard(NULL)
,	m_pMouse(NULL)
	//
,	m_pRootSceneNode(NULL)
{
	//nothing?
}

void Application::Run()
{
	//create application
	create();

	//main application loop
	Ogre::Timer clock;
	float lastTime = 0.f;
	while(Update(lastTime))
	{
		//handle all updates elsewhere
		lastTime = float(clock.getMilliseconds()) / 1000.f;
		clock.reset();

		Sleep(1);
	}
}

bool Application::frameStarted(const Ogre::FrameEvent& evt)
{
	/*if(renderCanvas)
		m_pCanvas->RenderCanvas();*/

	EffectManager::GetSingleton().frameStarted(evt);

	return true;
}

bool Application::frameEnded(const Ogre::FrameEvent& evt)
{
	return true;
}

bool Application::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(m_pWindow->isClosed())
        return false;
 
    if(m_ShutdownNextUpdate)
        return false;
 
    //Need to capture/update each device
    m_pKeyboard->capture();
    m_pMouse->capture();
 
    //Need to inject timestamps to CEGUI System.
    CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);
	
	//lastly, run game logic
	//m_GameManager.Update(evt.timeSinceLastFrame);

    return true;
}

Ogre::SceneManager& Application::GetSceneManager()
{
	return *m_pSceneManager;
}

Ogre::SceneManager& Application::StaticGetSceneManager()
{
	return *Application::GetSingleton().m_pSceneManager;
}

btDiscreteDynamicsWorld& Application::GetDynamicsWorld()
{
	return *this->m_pBulletDynamicsWorld;
}

btDiscreteDynamicsWorld& Application::StaticGetDynamicsWorld()
{
	return *Application::GetSingleton().m_pBulletDynamicsWorld;
}

CEGUI::Window& Application::GetCEGUIWindow()
{
	return *m_pCEGUIWindow;
}

CEGUI::Window& Application::StaticGetCEGUIWindow()
{
	return *Application::GetSingleton().m_pCEGUIWindow;
}

Ogre::SceneNode& Application::GetRootSceneNode()
{
	return *m_pRootSceneNode;
}

Ogre::SceneNode& Application::StaticGetRootSceneNode()
{
	return *Application::GetSingleton().m_pRootSceneNode;
}

Ogre::RenderWindow& Application::GetRenderWindow()
{
	return *m_pWindow;
}

Ogre::RenderWindow& Application::StaticGetRenderWindow()
{
	return *Application::GetSingleton().m_pWindow;
}

void Application::RegisterKeyListener(OIS::KeyListener* a_pKeyListener)
{
	m_pKeyboard->setEventCallback(a_pKeyListener);
}

void Application::StaticRegisterKeyListener(OIS::KeyListener* a_pKeyListener)
{
	Application::GetSingleton().m_pKeyboard->setEventCallback(a_pKeyListener);
}

void Application::RegisterMouseListener(OIS::MouseListener* a_pMouseListener)
{
	Application::GetSingleton().m_pMouse->setEventCallback(a_pMouseListener);
}

void Application::StaticRegisterMouseListener(OIS::MouseListener* a_pMouseListener)
{
	Application::GetSingleton().m_pMouse->setEventCallback(a_pMouseListener);
}
