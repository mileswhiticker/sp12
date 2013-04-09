#ifndef HUMAN_HPP
#define HUMAN_HPP

#include "Mob.hpp"

class Human
:	public Mob
{
public:
	Human(Ogre::Vector3 a_StartPos, int a_Direction = 0);
	virtual void InstantiateAtom();
	virtual void InstantiateMob();
	//
	virtual void Update(float a_DeltaT);
	//
private:
	//
};

#endif	HUMAN_HPP