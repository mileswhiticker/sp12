#ifndef GIRDER_BUILD_HPP
#define GIRDER_BUILD_HPP

#include "Context.hpp"

class Turf;
class Mob;
class Atom;
//
class btRigidBody;
class btBoxShape;

class GirderBuild
:	public Context
{
public:
	GirderBuild(Turf* a_pLocTurf, Atom* a_pOwnedAtom);
	virtual bool AttemptInteract(Mob* a_pSourceMob, Atom* a_pUsedAtom, int a_InteractType);
	//

};

#endif	GIRDER_BUILD_HPP