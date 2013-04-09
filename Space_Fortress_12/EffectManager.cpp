#include "EffectManager.hpp"

#include <omp.h>

#include "DebugDrawer.h"
#include "Cached.hpp"

bool EffectManager::frameStarted(const Ogre::FrameEvent& evt)
{
	if(m_NeedsRebuild)
	{
		for(auto it = m_CachedSpheres.begin(); it != m_CachedSpheres.end(); ++it)
		{
			DrawSphere(*it);
			//delete *it;
		}
		//m_CachedSpheres.clear();
	
		for(auto it = m_CachedLines.begin(); it != m_CachedLines.end(); ++it)
		{
			DrawLine(*it);
			//delete *it;
		}
		//m_CachedLines.clear();
		
		for(auto it = m_CachedCubes.begin(); it != m_CachedCubes.end(); ++it)
		{
			DrawCube(*it);
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
	while(m_CachedSpheres.size() > 50)
	{
		m_CachedSpheres.erase(m_CachedSpheres.begin());
	}
	m_NeedsRebuild = true;
}

void EffectManager::CacheLine(CachedLine* a_pCachedLine)
{
	m_CachedLines.push_back(a_pCachedLine);
	while(m_CachedLines.size() > 50)
	{
		m_CachedLines.erase(m_CachedLines.begin());
	}
	m_NeedsRebuild = true;
}

void EffectManager::CacheCube(CachedCube* a_pCachedCube)
{
	m_CachedCubes.push_back(a_pCachedCube);
	while(m_CachedCubes.size() > 50)
	{
		m_CachedCubes.erase(m_CachedCubes.begin());
	}
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

void EffectManager::DrawSphere(CachedSphere* a_pCachedSphere)
{
	DebugDrawer::getSingleton().drawSphere(a_pCachedSphere->pos, a_pCachedSphere->radius, a_pCachedSphere->colour, true);
}

void EffectManager::DrawLine(CachedLine* a_pCachedLine)
{
	DebugDrawer::getSingleton().drawLine(a_pCachedLine->startPos, a_pCachedLine->endPos, a_pCachedLine->colour);
}

void EffectManager::DrawCube(CachedCube* a_pCachedCube)
{
	//positive Z and negative Z
	Ogre::Vector3 posZ[4] = {a_pCachedCube->m_Verts[0] + a_pCachedCube->pos, a_pCachedCube->m_Verts[1] + a_pCachedCube->pos, a_pCachedCube->m_Verts[2] + a_pCachedCube->pos, a_pCachedCube->m_Verts[3] + a_pCachedCube->pos};
	DebugDrawer::getSingleton().drawQuad(posZ, a_pCachedCube->colour, true);
	Ogre::Vector3 negZ[4] = {a_pCachedCube->m_Verts[4] + a_pCachedCube->pos, a_pCachedCube->m_Verts[5] + a_pCachedCube->pos, a_pCachedCube->m_Verts[6] + a_pCachedCube->pos, a_pCachedCube->m_Verts[7] + a_pCachedCube->pos};
	DebugDrawer::getSingleton().drawQuad(negZ, a_pCachedCube->colour, true);
			
	//positive Y
	Ogre::Vector3 posY[4] = {a_pCachedCube->m_Verts[0] + a_pCachedCube->pos, a_pCachedCube->m_Verts[1] + a_pCachedCube->pos, a_pCachedCube->m_Verts[4] + a_pCachedCube->pos, a_pCachedCube->m_Verts[5] + a_pCachedCube->pos};
	DebugDrawer::getSingleton().drawQuad(posY, a_pCachedCube->colour, true);

	//negative Y
	Ogre::Vector3 negY[4] = {a_pCachedCube->m_Verts[2] + a_pCachedCube->pos, a_pCachedCube->m_Verts[3] + a_pCachedCube->pos, a_pCachedCube->m_Verts[6] + a_pCachedCube->pos, a_pCachedCube->m_Verts[7] + a_pCachedCube->pos};
	DebugDrawer::getSingleton().drawQuad(negY, a_pCachedCube->colour, true);
}

void EffectManager::ForceRebuild()
{
	m_NeedsRebuild = true;
}
