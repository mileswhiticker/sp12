#include "GlueScreen.hpp"
#include "Button.hpp"

#include <iostream>

#include <OGRE\OgreRoot.h>
#include <OGRE\OgreRenderWindow.h>
#include <OGRE\OgreVector2.h>

#include <cegui\CEGUIWindowManager.h>
#include <cegui\CEGUISystem.h>
#include <cegui\CEGUIWindow.h>
#include <cegui\CEGUIImage.h>
#include <cegui\CEGUIImagesetManager.h>
#include <cegui\elements\CEGUIPushButton.h>

MainMenu::MainMenu(bool a_BootstrapImmediately)
:	m_pNewGameButton(NULL)
,	m_pQuitButton(NULL)
,	m_pMenuTitle(NULL)
,	m_pMenuBackground(NULL)
,	m_pAuthorTitle(NULL)
{
	Create();
}

void MainMenu::Create()
{
	//
	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::System& system = CEGUI::System::getSingleton();
	CEGUI::ImagesetManager& imsMgr = CEGUI::ImagesetManager::getSingleton();
	
	CEGUI::UDim& windowHeight = system.getGUISheet()->getHeight();
	CEGUI::UDim& windowWidth = system.getGUISheet()->getWidth();
	CEGUI::UDim posX;
	posX.d_offset = 200;
	CEGUI::UDim posY;
	posY.d_offset = 200;
	
	/*imsMgr.createFromImageFile("MenuWallpaper", "menu.jpg", "Imagesets");
	
	m_pMenuBackground = wmgr.createWindow("TaharezLook/StaticImage", "Menu/Wallpaper");
	m_pMenuBackground->setProperty("Image", "set:MenuWallpaper image:full_image");
	m_pMenuBackground->setSize(CEGUI::UVector2(CEGUI::UDim(1.f, 0), CEGUI::UDim(1.f, 0)));
	m_pMenuBackground->setEnabled(false);
	system.getGUISheet()->addChildWindow(m_pMenuBackground);*/
	
	/*m_pMenuTitle = wmgr.createWindow("TaharezLook/StaticText", "Menu/GameTitle");
	m_pMenuTitle->setText("some kind of a game by Cael_Aislinn");
	m_pMenuTitle->setSize(CEGUI::UVector2(CEGUI::UDim(0.4f, 0), CEGUI::UDim(0.1f, 0)));
	m_pMenuTitle->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55f, 0), CEGUI::UDim(0.85f, 0)));
	system.getGUISheet()->addChildWindow(m_pMenuTitle);*/

	/*m_pAuthorTitle = wmgr.createWindow("TaharezLook/StaticText", "Menu/AuthorTitle");
	m_pAuthorTitle->setText("Cael_Aislinn");
	m_pAuthorTitle->setSize(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.05f, 0)));
	m_pAuthorTitle->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.85f, 0)));
	system.getGUISheet()->addChildWindow(m_pAuthorTitle);*/

	/*m_pNewGameButton = wmgr.createWindow("TaharezLook/Button", "Menu/NewGameButton");
	m_pNewGameButton->setText("New Game");
	m_pNewGameButton->setSize(CEGUI::UVector2(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.05f, 0)));
	m_pNewGameButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.2f, 0)));
	system.getGUISheet()->addChildWindow(m_pNewGameButton);*/

	/*m_pQuitButton = wmgr.createWindow("TaharezLook/Button", "Menu/QuitButton");
	m_pQuitButton->setText("Quit");
	m_pQuitButton->setSize(CEGUI::UVector2(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.05f, 0)));
	m_pQuitButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.4f, 0)));
	system.getGUISheet()->addChildWindow(m_pQuitButton);
	//m_pQuitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&BasicTutorial7::quit, this));*/
}

void MainMenu::Update(float a_DeltaT)
{
	//
}

void MainMenu::Destroy()
{
	//todo
}
