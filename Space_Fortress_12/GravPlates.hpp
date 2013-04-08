#ifndef GRAV_PLATES_HPP
#define GRAV_PLATES_HPP

#include "Structure.hpp"

class GravPlates
:	public Structure
{
public:
	GravPlates(Ogre::Vector3 a_Pos, int a_Dir = 0);
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

#endif	GRAV_PLATES_HPP