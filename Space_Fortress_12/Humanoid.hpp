#ifndef HUMANOID_HPP
#define HUMANOID_HPP

#include "Mob.hpp"

class Humanoid
:	public Mob
{
public:
	Humanoid(Ogre::Vector3 a_StartPos, int a_Direction = 0);
	virtual void InstantiateAtom();
	//
	virtual void Update(float a_DeltaT);
	//
private:
	//
};

#endif	HUMANOID_HPP