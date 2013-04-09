#ifndef EFFECT_MANAGER_HPP
#define EFFECT_MANAGER_HPP

struct CachedLine;
struct CachedSphere;
struct CachedCube;
#include <vector>
#include <OGRE\OgreFrameListener.h>

class EffectManager
:	public Ogre::FrameListener
{
public:
	static EffectManager& GetSingleton()
	{
		return *GetSingletonPtr();
	};
	static EffectManager* GetSingletonPtr()
	{
		static EffectManager* pInstance = new EffectManager();
		return pInstance;
	};
	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual bool frameEnded(const Ogre::FrameEvent& evt);
	//
	void CacheSphere(CachedSphere* a_pCachedSphere);
	void CacheLine(CachedLine* a_pCachedLine);
	void CacheCube(CachedCube* a_pCachedCube);
	void ClearCacheSphere(CachedSphere* a_pCachedSphere);
	void ClearCacheLine(CachedLine* a_pCachedLine);
	void ClearCacheCube(CachedCube* a_pCachedCube);
	//
	void ForceRebuild();
private:
	EffectManager()
	:	m_NeedsRebuild(false)
	{};
	bool m_NeedsRebuild;
	//
	std::vector<CachedLine*> m_CachedLines;
	std::vector<CachedSphere*> m_CachedSpheres;
	std::vector<CachedCube*> m_CachedCubes;
	//
};

#endif	EFFECT_MANAGER_HPP