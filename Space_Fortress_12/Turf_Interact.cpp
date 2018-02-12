#include "Turf.hpp"

#include <stdio.h>

bool Turf::AttemptInteract(Mob* a_pSourceMob, Atom* a_pUsedAtom, int a_InteractType)
{
	std::cout << "DEBUG: Turf::AttemptInteract()" << std::endl;

	/*
	switch(a_InteractType)
	{
	case(Mob::TARGET):
		{
			if(m_pTargetEntity && m_pSceneNode && !m_pTargetEntity->getParentSceneNode())
			{
				m_pSceneNode->attachObject(m_pTargetEntity);
			}
			break;
		}
	case(Mob::UNTARGET):
		{
			if(m_pTargetEntity && m_pSceneNode && m_pTargetEntity->getParentSceneNode())
			{
				m_pSceneNode->detachObject(m_pTargetEntity);
			}
			break;
		}
	case(Mob::INTERACT):
		{
			Ogre::SceneNode* pSceneNode = m_pTargetEntity->getParentSceneNode();
			if(pSceneNode)
			{
				pSceneNode->detachObject(m_pTargetEntity);
			}
			break;
		}
	default:
		{
			return Context::AttemptInteract(a_pSourceMob, a_pUsedAtom, a_InteractType);
		}
	}
	*/

	return true;
}

bool Turf::Interact(Mob* a_pSourceMob, Context* a_pSourceContext, int a_InteractType, Atom* a_pUsedAtom)
{
	switch(a_pSourceContext->GetContextType())
	{
	case(Context::BUILD):
		{
			if(!m_MyTurfType)
			{
				Instantiate(Turf::GIRDER);
				return true;
			}
			return false;
		}
	case(Context::UNBUILD):
		{
			if(m_MyTurfType)
			{
				Instantiate(Turf::BUILDTURF);
				return true;
			}
			return false;
		}
	}

	//just pass it up the chain
	return Atom::Interact(a_pSourceMob, a_pSourceContext, a_InteractType, a_pUsedAtom);
}
