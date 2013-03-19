#include "OgreHelper.hpp"

#include "Application.hpp"

Ogre::SceneNode& GetRootSceneNode()
{
	return Application::GetSingleton().StaticGetRootSceneNode();
}

Ogre::SceneNode* NewSceneNode()
{
	return Application::GetSingleton().StaticGetRootSceneNode().createChildSceneNode();
}

Ogre::SceneManager& GetSceneManager()
{
	return Application::GetSingleton().StaticGetSceneManager();
}

Ogre::RenderWindow& GetRenderWindow()
{
	return Application::GetSingleton().StaticGetRenderWindow();
}

void RegisterKeyListener(OIS::KeyListener* a_pKeyListener)
{
	Application::GetSingleton().StaticRegisterKeyListener(a_pKeyListener);
}

void RegisterMouseListener(OIS::MouseListener* a_pMouseListener)
{
	Application::GetSingleton().StaticRegisterMouseListener(a_pMouseListener);
}
