#ifndef GRAV_PLATES_HPP
#define GRAV_PLATES_HPP

#include "Structure.hpp"
#include "GravitySource.hpp"

class GravPlates
:	public Structure
,	public GravitySource
{
public:
	GravPlates(MapCell* a_pMapCell, int a_Dir = 0);
	virtual void InstantiateStructure(bool a_IsBuildPoint);
	virtual void CreateFromBuildPoint();
	virtual void DestroyToBuildPoint();
	//
	virtual void Select(ObserverBuild* a_pSelectingObserver);
	virtual void DeSelect(ObserverBuild* a_pSelectingObserver);
	//
	void ToggleGravity();
	void SetGravity(bool a_Active = true);
	void SetPercentGravityFalloff(float a_NewLinearFallof);
private:
	float m_PercentGravityFalloff;
	bool m_GravityActive;
	//
};

#endif	GRAV_PLATES_HPP