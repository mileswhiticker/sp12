#ifndef UNDERLAY_PLATING_HPP
#define UNDERLAY_PLATING_HPP

#include "Structure.hpp"
#include <OGRE\OgreVector3.h>

class Turf;
namespace Ogre
{
	class SceneNode;
};

class UnderlayPlating
:	public Structure
{
public:
	UnderlayPlating(Turf* a_pLocTurf, int a_Dir = 0);
	virtual void InstantiateStructure(bool a_IsBuildPoint);
	virtual void CreateFromBuildPoint();
	virtual void DestroyToBuildPoint();
	//
	virtual void Select(InputModule* a_pSelectingInputModule);
	virtual void DeSelect(InputModule* a_pSelectingInputModule);
	//
};

#endif	UNDERLAY_PLATING_HPP