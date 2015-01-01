#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include "Mob.hpp"
class PossessMob;
class RCD;

class Observer
:	public Mob
{
public:
	Observer(Ogre::Vector3 a_StartPos, int a_Direction = 0);
	virtual void InstantiateAtom();
	//
	void SetTargetPossessMob(Mob* a_pTargetPossessMob);
	//
protected:
	PossessMob* m_pPossessMob;
	//
	RCD* m_pMyRCD;
};

#endif	OBSERVER_HPP