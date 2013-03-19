#include "Mob.hpp"

#include "InputModule.hpp"
#include "Client.hpp"
#include "OgreHelper.hpp"

Mob::Mob()
:	m_pPossessingClient(NULL)
{
	Ogre::SceneNode& rootSceneNode = GetRootSceneNode();
	m_pAtomSceneNode = rootSceneNode.createChildSceneNode();
}

void Mob::Update(float a_DeltaT)
{
	for(auto it = m_InputModules.begin(); it != m_InputModules.end(); ++it)
	{
		(*it)->Update(a_DeltaT);
	}
}

bool Mob::ConnectClient(Client* a_pNewClient)
{
	if(!m_pPossessingClient)
	{
		m_pPossessingClient = a_pNewClient;
		m_pPossessingClient->m_pPossessedMob = this;
		m_pPossessingClient->ResetCamera();

		RegisterKeyListener(this);
		RegisterMouseListener(this);
		
		for(auto it = m_InputModules.begin(); it != m_InputModules.end(); ++it)
		{
			(*it)->SetClient(a_pNewClient);
		}

		return true;
	}
	return false;
}

bool Mob::DisconnectClient()
{
	if(m_pPossessingClient)
	{
		m_pPossessingClient = NULL;

		for(auto it = m_InputModules.begin(); it != m_InputModules.end(); ++it)
		{
			(*it)->SetClient(NULL);
		}

		return true;
	}
	return false;
}

bool Mob::keyPressed( const OIS::KeyEvent &arg )
{
	for(auto it = m_InputModules.begin(); it != m_InputModules.end(); ++it)
	{
		(*it)->keyPressed(arg);
	}

	return false;
}

bool Mob::keyReleased( const OIS::KeyEvent &arg )
{
	for(auto it = m_InputModules.begin(); it != m_InputModules.end(); ++it)
	{
		(*it)->keyReleased(arg);
	}

	return false;
}

bool Mob::mouseMoved( const OIS::MouseEvent &arg )
{
	for(auto it = m_InputModules.begin(); it != m_InputModules.end(); ++it)
	{
		(*it)->mouseMoved(arg);
	}

	return false;
}

bool Mob::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	for(auto it = m_InputModules.begin(); it != m_InputModules.end(); ++it)
	{
		(*it)->mousePressed(arg, id);
	}

	return false;
}
bool Mob::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	for(auto it = m_InputModules.begin(); it != m_InputModules.end(); ++it)
	{
		(*it)->mouseReleased(arg, id);
	}

	return false;
}
