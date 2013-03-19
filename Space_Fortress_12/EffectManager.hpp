#ifndef EFFECT_MANAGER_HPP
#define EFFECT_MANAGER_HPP

struct CachedLine;
struct CachedSphere;
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
	//
private:
	EffectManager(){};
	//
	std::vector<CachedLine*> m_CachedLines;
	std::vector<CachedSphere*> m_CachedSpheres;
	//
};

#endif	EFFECT_MANAGER_HPP