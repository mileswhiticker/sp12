#ifndef STRUCTURE_HPP
#define STRUCTURE_HPP

#include "Atom.hpp"

class Girder;
class Turf;
class InputModule;

class Structure
:	public Atom
{
public:
	enum StructureType
	{
		UNKNOWN = 0,
		OVERLAYPLATING,
		UNDERLAYPLATING,
		LIGHTFIXTURE,
		GRAVPLATES
		//
	};
	Structure(Turf* a_pLocTurf, int a_Dir = 0);
	//
	virtual void InstantiateAtom();
	virtual void InstantiateStructure(bool a_IsBuildPoint)=0;
	//
	virtual void CreateFromBuildPoint()=0;
	virtual void DestroyToBuildPoint()=0;
	virtual bool IsBuildPoint();
	//
	virtual void Interact(Atom* a_pSourceAtom, InputModule* a_pSourceModule, int a_Intent, int a_Type = 0);
	virtual void Select(InputModule* a_pSelectingInputModule);
	virtual void DeSelect(InputModule* a_pSelectingInputModule);
	//
	bool MountOnGirder(Girder* a_pMountTarget);
	bool UnmountFromGirder();
	//
	StructureType GetStructureType();
	//
protected:
	StructureType m_MyStructureType;
	Girder* m_pMountedGirder;
	Structure* m_pMountedOnStructure;
	bool m_IsBuildPoint;
	//
};

#endif	STRUCTURE_HPP