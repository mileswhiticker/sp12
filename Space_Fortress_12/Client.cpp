#include "Client.hpp"

#include "Application.hpp"

#include <CEGUI\CEGUIWindowManager.h>
#include <CEGUI\elements\CEGUIGUISheet.h>

#include <OGRE\OgreCamera.h>
#include <OGRE\OgreSceneNode.h>

#include "OgreHelper.hpp"

#include "Mob.hpp"

Client::Client()
:	m_pCamera(NULL)
,	m_pCameraNode(NULL)
,	m_pPossessedMob(NULL)
,	m_HasPersonalCameraNode(true)
{
	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();

	//test window
	m_pTopInfoBar = (CEGUI::GUISheet*)wmgr.createWindow( "TaharezLook/StaticText", "testWindow" );
	Application::StaticGetCEGUIWindow().addChildWindow(m_pTopInfoBar);
	m_pTopInfoBar->setPosition( CEGUI::UVector2( CEGUI::UDim( 0, 0 ), CEGUI::UDim( 0, 0 ) ) );
	m_pTopInfoBar->setSize( CEGUI::UVector2( CEGUI::UDim( 1, 0 ), CEGUI::UDim( 0.05f, 0 ) ) );
	m_pTopInfoBar->setText("Observer cam");
}

void Client::Update(float a_DeltaT)
{
	//
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
		m_pCamera->lookAt(0.f, 0.f, 0.f);

		// Create one viewport, entire window
		Ogre::Viewport* vp = renderWindow.addViewport(m_pCamera);

		// Alter the camera aspect ratio to match the viewport
		m_pCamera->setAspectRatio((float)vp->getActualWidth() / (float)vp->getActualHeight());
	}
	
	if(m_pPossessedMob && m_pPossessedMob->m_pAtomSceneNode)
	{
		//clear out the old node, and use the mob's one
		if(m_pCameraNode)
		{
			m_pCameraNode->detachObject(m_pCamera);
			if(m_HasPersonalCameraNode)
			{
				//we created this one, so we have to delete it
				Ogre::SceneNode* pCameraNodeParent = m_pCameraNode->getParentSceneNode();
				if(pCameraNodeParent)
				{
					pCameraNodeParent->removeChild(m_pCameraNode);
				}
				delete pCameraNodeParent;
			}
			m_pCameraNode = NULL;
		}

		//set the new node
		m_HasPersonalCameraNode = false;
		m_pCameraNode = m_pPossessedMob->m_pAtomSceneNode;
		m_pCameraNode->attachObject(m_pCamera);
	}
	else if(!m_pCameraNode)
	{
		//create our own node
		m_pCameraNode = rootSceneNode.createChildSceneNode("client_camera_node");
		m_pCameraNode->attachObject(m_pCamera);
		m_HasPersonalCameraNode = true;
	}
}
