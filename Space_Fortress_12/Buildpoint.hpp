#ifndef BUILDPOINT_HPP
#define BUILDPOINT_HPP

class Turf;
class Structure;

struct Buildpoint
{
	enum Position
	{
		UNKNOWN = 0,
		//
		INNER_PLATING_OUT,
		INNER_PLATING_IN,
		OUTER_PLATING_OUT,
		OUTER_PLATING_IN,
		//
		POS_MAXVAL
	};
	Buildpoint(Turf& a_LocTurf, int a_Position, int a_Dir, bool a_BlockingAbove = true)
	:	m_LocTurf(a_LocTurf)
	,	m_Position(a_Position)
	,	m_Dir(a_Dir)
	,	m_BlockingAbove(a_BlockingAbove)
	,	m_pHeldStructure(0)
	{
		//
	}
	//
	Turf& m_LocTurf;
	Turf* m_pHeldStructure;
	int m_Dir;				//our direction relative to the turf we're on
	int m_Position;	
	bool m_BlockingAbove;	//whether something can be placed directly above us
							//this includes additional plating or structures, and extends to the adjacent turf
};

#endif	BUILDPOINT_HPP