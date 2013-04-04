#ifndef UNDERLAY_PLATING_HPP
#define UNDERLAY_PLATING_HPP

#include "Structure.hpp"
#include <OGRE\OgreVector3.h>

namespace Ogre
{
	class SceneNode;
};

class UnderlayPlating
:	public Structure
{
public:
	UnderlayPlating(Ogre::Vector3 a_Pos);
	virtual void InstantiateStructure(bool a_IsBuildPoint);
	virtual void CreateFromBuildPoint();
	virtual void DestroyToBuildPoint();
	//
private:
	Ogre::SceneNode* m_pActualSceneNode;
	//
};

#endif	UNDERLAY_PLATING_HPP