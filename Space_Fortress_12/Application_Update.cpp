#include "Application.hpp"

#include <OGRE\OgreRoot.h>
#include <OGRE\OgreFrameListener.h>
#include "BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h"

#include "Client.hpp"
#include "Mob.hpp"
#include "AtomManager.hpp"

bool Application::Update(float a_DeltaT)
{
	//get window messages
	MSG  msg;
	while( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
	{
		if (msg.message == WM_QUIT)
			m_ShutdownNextUpdate = true;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	//update connected clients
	for(auto it = m_ConnectedClients.begin(); it != m_ConnectedClients.end(); ++it)
	{
		(*it)->Update(a_DeltaT);
	}

	//update mobs on server
	for(auto it = m_Mobs.begin(); it != m_Mobs.end(); ++it)
	{
		(*it)->Update(a_DeltaT);
	}

	m_pBulletDynamicsWorld->stepSimulation(a_DeltaT, 100);
	AtomManager::GetSingleton().Update(a_DeltaT);

	//render a single frame
	return (!m_ShutdownNextUpdate && m_pRoot->renderOneFrame());
}
