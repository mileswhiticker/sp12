#ifndef OVERLAY_PLATING_HPP
#define OVERLAY_PLATING_HPP

#include "Structure.hpp"
#include <OGRE\OgreVector3.h>

namespace Ogre
{
	class SceneNode;
};

class OverlayPlating
:	public Structure
{
public:
	OverlayPlating(Ogre::Vector3 a_Pos);
	virtual void InstantiateStructure(bool a_IsBuildPoint);
	virtual void CreateFromBuildPoint();
	//
private:
	Ogre::SceneNode* m_pActualSceneNode;
	//
};

#endif	OVERLAY_PLATING_HPP