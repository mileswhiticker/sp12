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
	UnderlayPlating(MapCell* a_pMapCell, int a_Dir = 0);
	virtual void InstantiateStructure(bool a_IsBuildPoint);
	virtual void CreateFromBuildPoint();
	virtual void DestroyToBuildPoint();
	//
	virtual void Select(ObserverBuild* a_pSelectingObserver);
	virtual void DeSelect(ObserverBuild* a_pSelectingObserver);
	//
};

#endif	UNDERLAY_PLATING_HPP