#include "EffectManager.hpp"

#include "DebugDrawer.h"
#include "Cached.hpp"

bool EffectManager::frameStarted(const Ogre::FrameEvent& evt)
{
	if(m_NeedsRebuild)
	{
		for(auto it = m_CachedSpheres.begin(); it != m_CachedSpheres.end(); ++it)
		{
			DebugDrawer::getSingleton().drawSphere((*it)->pos, (*it)->radius, (*it)->colour, true);
			//delete *it;
		}
		//m_CachedSpheres.clear();
	
		for(auto it = m_CachedLines.begin(); it != m_CachedLines.end(); ++it)
		{
			DebugDrawer::getSingleton().drawLine((*it)->startPos, (*it)->endPos, (*it)->colour);
			//delete *it;
		}
		//m_CachedLines.clear();

		for(auto it = m_CachedCubes.begin(); it != m_CachedCubes.end(); ++it)
		{
			//positive Z and negative Z
			Ogre::Vector3 posZ[4] = {(*it)->m_Verts[0] + (*it)->pos, (*it)->m_Verts[1] + (*it)->pos, (*it)->m_Verts[2] + (*it)->pos, (*it)->m_Verts[3] + (*it)->pos};
			DebugDrawer::getSingleton().drawQuad(posZ, (*it)->colour, true);
			Ogre::Vector3 negZ[4] = {(*it)->m_Verts[4] + (*it)->pos, (*it)->m_Verts[5] + (*it)->pos, (*it)->m_Verts[6] + (*it)->pos, (*it)->m_Verts[7] + (*it)->pos};
			DebugDrawer::getSingleton().drawQuad(negZ, (*it)->colour, true);
			
			//positive Y
			Ogre::Vector3 posY[4] = {(*it)->m_Verts[0] + (*it)->pos, (*it)->m_Verts[1] + (*it)->pos, (*it)->m_Verts[4] + (*it)->pos, (*it)->m_Verts[5] + (*it)->pos};
			DebugDrawer::getSingleton().drawQuad(posY, (*it)->colour, true);

			//negative Y
			Ogre::Vector3 negY[4] = {(*it)->m_Verts[2] + (*it)->pos, (*it)->m_Verts[3] + (*it)->pos, (*it)->m_Verts[6] + (*it)->pos, (*it)->m_Verts[7] + (*it)->pos};
			DebugDrawer::getSingleton().drawQuad(negY, (*it)->colour, true);

			//delete *it;
		}
		//m_CachedCubes.clear();
		
		m_NeedsRebuild = false;
		DebugDrawer::getSingleton().build();
	}
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
	m_NeedsRebuild = true;
}

void EffectManager::CacheLine(CachedLine* a_pCachedLine)
{
	m_CachedLines.push_back(a_pCachedLine);
	m_NeedsRebuild = true;
}

void EffectManager::CacheCube(CachedCube* a_pCachedCube)
{
	m_CachedCubes.push_back(a_pCachedCube);
	m_NeedsRebuild = true;
}

void EffectManager::ClearCacheSphere(CachedSphere* a_pCachedSphere)
{
	for(auto it = m_CachedSpheres.begin(); it != m_CachedSpheres.end(); ++it)
	{
		if((*it) == a_pCachedSphere)
		{
			//delete *it;
			m_CachedSpheres.erase(it);
			m_NeedsRebuild = true;
			return;
		}
	}
}

void EffectManager::ClearCacheLine(CachedLine* a_pCachedLine)
{
	for(auto it = m_CachedLines.begin(); it != m_CachedLines.end(); ++it)
	{
		if((*it) == a_pCachedLine)
		{
			//delete *it;
			m_CachedLines.erase(it);
			m_NeedsRebuild = true;
			return;
		}
	}
}

void EffectManager::ClearCacheCube(CachedCube* a_pCachedCube)
{
	for(auto it = m_CachedCubes.begin(); it != m_CachedCubes.end(); ++it)
	{
		if((*it) == a_pCachedCube)
		{
			//delete *it;
			m_CachedCubes.erase(it);
			m_NeedsRebuild = true;
			return;
		}
	}
}

void EffectManager::ForceRebuild()
{
	m_NeedsRebuild = true;
}
