#ifndef RCD_HPP
#define RCD_HPP

#include "Object.hpp"
#include "InputModule.hpp"

class Structure;
class RCD_InputModule;

class RCD
:	public Object
{
public:
	RCD(Ogre::Vector3 a_StartPos, int a_StartDirection = 0);

	friend class RCD_InputModule;
	//
private:
	int m_CellBuildRange;
	int m_TargetAtomTypes;
	int m_TargetAtomSubTypes;
	//
	//int m_BuildOptionsIndex;
	//std::vector<Atom*> m_BuildOptions;
	//
};

#endif	RCD_HPP