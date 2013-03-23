#ifndef STRUCTURE_HPP
#define STRUCTURE_HPP

#include <list>
#include "Atom.hpp"

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
	void OwnedBuildPointCreated(Structure* a_pChild);
	//
protected:
	StructureType m_MyStructureType;
	bool m_IsBuildPoint;
	Structure* m_pMountedOnStructure;
	std::list<Structure*> m_UsedBuildPoints;
	std::list<Structure*> m_UnusedBuildPoints;
	//
};

#endif	STRUCTURE_HPP