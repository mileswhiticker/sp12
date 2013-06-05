#include "Client.hpp"

#include "Application.hpp"
#include "Mob.hpp"

#include <CEGUI\CEGUIWindowManager.h>
#include <CEGUI\elements\CEGUIGUISheet.h>

#include <OGRE\OgreCamera.h>
#include <OGRE\OgreSceneNode.h>

#include "OgreHelper.hpp"
#include "MapHelper.hpp"
#include "num2string.h"

Client::Client()
:	m_pCamera(NULL)
,	m_pCameraNode(NULL)
,	m_pPossessedMob(NULL)
,	m_HasPersonalCameraNode(true)
{
	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();

	//test window
	m_pTopInfoBar = (CEGUI::GUISheet*)wmgr.createWindow( "TaharezLook/StaticText", "topInfoBar" );
	Application::StaticGetCEGUIWindow().addChildWindow(m_pTopInfoBar);
	m_pTopInfoBar->setPosition( CEGUI::UVector2( CEGUI::UDim( 0, 0 ), CEGUI::UDim( 0, 0 ) ) );
	m_pTopInfoBar->setSize( CEGUI::UVector2( CEGUI::UDim( 0.5f, 0 ), CEGUI::UDim( 0.05f, 0 ) ) );
	m_pTopInfoBar->setText("Observer cam");

	//fps counter
	m_pFPSCounter = (CEGUI::GUISheet*)wmgr.createWindow( "TaharezLook/StaticText", "fpsCounter" );
	Application::StaticGetCEGUIWindow().addChildWindow(m_pFPSCounter);
	m_pFPSCounter->setPosition( CEGUI::UVector2( CEGUI::UDim( 0.5f, 0 ), CEGUI::UDim( 0, 0 ) ) );
	m_pFPSCounter->setSize( CEGUI::UVector2( CEGUI::UDim( 0.5f, 0 ), CEGUI::UDim( 0.05f, 0 ) ) );
	m_pFPSCounter->setText("FPS: NA");
}

void Client::Update(float a_DeltaT)
{
	m_pFPSCounter->setText("FPS: " + num2string(1.f/a_DeltaT));
}

void Client::Login()
{
	//
}

void Client::Logout()
{
	//
}

void Client::ResetCamera()
{
	Ogre::SceneNode& rootSceneNode = GetRootSceneNode();
	Ogre::SceneManager& sceneManager = GetSceneManager();
	Ogre::RenderWindow& renderWindow = GetRenderWindow();

	if(!m_pCamera)
	{
		m_pCamera = sceneManager.createCamera("client_camera");
		m_pCamera->setNearClipDistance(0.0001f);
		m_pCamera->setFarClipDistance(9999999.f);

		// Create one viewport, entire window
		Ogre::Viewport* vp = renderWindow.addViewport(m_pCamera);

		// Alter the camera aspect ratio to match the viewport
		m_pCamera->setAspectRatio((float)vp->getActualWidth() / (float)vp->getActualHeight());
	}
	
	if(!m_pCameraNode)
	{
		m_pCameraNode = rootSceneNode.createChildSceneNode("client_camera_node");
		m_pCameraNode->attachObject(m_pCamera);
		m_HasPersonalCameraNode = true;
	}
	if(m_pPossessedMob)
	{
		if(m_pPossessedMob->m_pAtomRootSceneNode)
		{
			//move our old node over to the new mob's root scene node
			if(m_pCameraNode->getParentSceneNode())
			{
				m_pCameraNode->getParentSceneNode()->removeChild(m_pCameraNode);
			}
			m_pPossessedMob->m_pAtomRootSceneNode->addChild(m_pCameraNode);
			m_pCameraNode->setPosition(m_pPossessedMob->GetCameraModelOffset());
		}

		//reorient the camera to match the direction of the mob
		Ogre::Vector3 lookDir = m_pPossessedMob->m_pAtomEntitySceneNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
		m_pCamera->lookAt(m_pCamera->getDerivedPosition() + lookDir);
	}
	else
	{
		//this should never occur
		m_pCamera->lookAt(m_pCamera->getDerivedPosition() + Ogre::Vector3::UNIT_Z);
	}
}
