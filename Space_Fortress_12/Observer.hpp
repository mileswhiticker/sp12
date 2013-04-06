#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include "Mob.hpp"

class Observer
:	public Mob
{
public:
	Observer(Ogre::Vector3 a_StartPos, int a_Direction = 0);
	virtual void InstantiateAtom();
	//
private:
	//
};

#endif	OBSERVER_HPP