#ifndef STRUCTURE_HPP
#define STRUCTURE_HPP

#include "Atom.hpp"

class Girder;

class Structure
:	public Atom
{
public:
	enum StructureType
	{
		UNKNOWN = 0,
		GIRDER,
		OVERLAYPLATING,
		UNDERLAYPLATING,
		//
	};
	Structure();
	//
	virtual void InstantiateAtom();
	virtual void InstantiateStructure(bool a_IsBuildPoint)=0;
	//
	bool IsBuildPoint();
	virtual void CreateFromBuildPoint()=0;
	virtual void DestroyToBuildPoint()=0;
	//
	bool MountOnGirder(Girder* a_pMountTarget);
	bool UnmountFromGirder();
	//
	StructureType GetStructureType();
	//
protected:
	StructureType m_MyStructureType;
	bool m_IsBuildPoint;
	Girder* m_pMountedGirder;
	Structure* m_pMountedOnStructure;
	//
};

#endif	STRUCTURE_HPP