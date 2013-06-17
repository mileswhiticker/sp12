#ifndef TURF_HPP
#define TURF_HPP

#include "Atom.hpp"

class MapCell;
class Structure;
class InputModule;

class Turf
:	public Atom
{
public:
	enum TurfType
	{
		UNKNOWN = 0,
		GIRDER
	};
	Turf(MapCell* a_pLocMapCell);
	//
	virtual void InstantiateAtom();
	virtual void InstantiateTurf(bool a_IsBuildPoint)=0;
	virtual void CreateFromBuildPoint()=0;
	virtual void DestroyToBuildPoint()=0;
	//
	TurfType GetTurfType();
	Structure* m_pTurfStructure;
	bool IsBuildPoint();
	//
	virtual void Interact(Atom* a_pSourceAtom, InputModule* a_pSourceModule, int a_Intent, int a_Type = 0);
	virtual void Select(InputModule* a_pSelectingInputModule);
	virtual void DeSelect(InputModule* a_pSelectingInputModule);
	//
protected:
	TurfType m_MyTurfType;
	bool m_IsBuildPoint;
	//
};

#endif	TURF_HPP