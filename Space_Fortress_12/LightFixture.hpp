#ifndef LIGHT_FIXTURE_HPP
#define LIGHT_FIXTURE_HPP

#include <OGRE/OgreVector3.h>

#include "Structure.hpp"

class Turf;
class LightFixture
:	public Structure
{
public:
	LightFixture(Turf* m_pLocTurf, int a_Dir = 0);

	virtual void InstantiateStructure(bool a_IsBuildPoint);
	virtual void CreateFromBuildPoint();
	virtual void DestroyToBuildPoint();
	//
	virtual void Select(InputModule* a_pSelectingInputModule);
	virtual void DeSelect(InputModule* a_pSelectingInputModule);
	//
private:
	//
};

#endif	LIGHT_FIXTURE_HPP