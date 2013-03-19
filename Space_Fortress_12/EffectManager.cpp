#include "EffectManager.hpp"

#include "DebugDrawer.h"
#include "Cached.hpp"

bool EffectManager::frameStarted(const Ogre::FrameEvent& evt)
{
	for(auto it = m_CachedSpheres.begin(); it != m_CachedSpheres.end(); ++it)
	{
		DebugDrawer::getSingleton().drawSphere((*it)->pos, (*it)->radius, (*it)->colour, true);
		delete *it;
	}
	m_CachedSpheres.clear();
	
	for(auto it = m_CachedLines.begin(); it != m_CachedLines.end(); ++it)
	{
		DebugDrawer::getSingleton().drawLine((*it)->startPos, (*it)->endPos, (*it)->colour);
		delete *it;
	}
	m_CachedLines.clear();

	DebugDrawer::getSingleton().build();
	return true;
}

bool EffectManager::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	//

	return true;
}

bool EffectManager::frameEnded(const Ogre::FrameEvent& evt)
{
	DebugDrawer::getSingleton().clear();
	
	return true;
}

void EffectManager::CacheSphere(CachedSphere* a_pCachedSphere)
{
	m_CachedSpheres.push_back(a_pCachedSphere);
}

void EffectManager::CacheLine(CachedLine* a_pCachedLine)
{
	m_CachedLines.push_back(a_pCachedLine);
}
