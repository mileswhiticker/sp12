#ifndef GRAV_PLATES_HPP
#define GRAV_PLATES_HPP

#include "Structure.hpp"

class GravPlates
:	public Structure
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
	void SetGravityRange(int a_NewRange);
	void SetGravityForce(float a_NewForce);
	void SetLinearGravityFalloff(float a_NewLinearFallof);
private:
	int m_CellRange;
	float m_InitialGravityForce;
	float m_LinearGravityFalloff;
	//
	bool m_GravityActive;
	bool m_GravityReversed;
	//
};

#endif	GRAV_PLATES_HPP