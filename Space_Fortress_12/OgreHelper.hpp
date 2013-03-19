#ifndef OGRE_HELPER_HPP
#define	OGRE_HELPER_HPP

#include <OGRE\OgreSceneNode.h>
#include <OGRE\OgreSceneManager.h>
#include <OGRE\OgreRenderWindow.h>
#include <OIS\OISKeyboard.h>
#include <OIS\OISMouse.h>

Ogre::SceneNode& GetRootSceneNode();
Ogre::SceneNode* NewSceneNode();
Ogre::SceneManager& GetSceneManager();
Ogre::RenderWindow& GetRenderWindow();
void RegisterKeyListener(OIS::KeyListener* a_pKeyListener);
void RegisterMouseListener(OIS::MouseListener* a_pMouseListener);

#endif	OGRE_HELPER_HPP