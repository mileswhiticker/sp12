#ifndef STRUCTURE_HPP
#define STRUCTURE_HPP

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
	virtual void Instantiate();
	virtual void Instantiate(bool a_IsBuildPoint)=0;
	//
	bool IsBuildPoint();
	//
protected:
	StructureType m_MyStructureType;
	bool m_IsBuildPoint;
	//
};

#endif	STRUCTURE_HPP