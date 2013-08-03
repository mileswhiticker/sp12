#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include "Mob.hpp"
class PossessMob;

class Observer
:	public Mob
{
public:
	Observer(Ogre::Vector3 a_StartPos, int a_Direction = 0);
	virtual void InstantiateAtom();
	virtual void SetupInputModules();
	//
	void SetTargetPossessMob(Mob* a_pTargetPossessMob);
	//
protected:
	PossessMob* m_pPossessMob;
	//
};

#endif	OBSERVER_HPP