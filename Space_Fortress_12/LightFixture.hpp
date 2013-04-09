#ifndef LIGHT_FIXTURE_HPP
#define LIGHT_FIXTURE_HPP

#include <OGRE/OgreVector3.h>

#include "Structure.hpp"
class LightFixture
:	public Structure
{
public:
	LightFixture(MapCell* m_pMapCell, int a_Dir = 0);

	virtual void InstantiateStructure(bool a_IsBuildPoint);
	virtual void CreateFromBuildPoint();
	virtual void DestroyToBuildPoint();
	//
	virtual void Select(ObserverBuild* a_pSelectingObserver);
	virtual void DeSelect(ObserverBuild* a_pSelectingObserver);
	//
private:
	//
};

#endif	LIGHT_FIXTURE_HPP