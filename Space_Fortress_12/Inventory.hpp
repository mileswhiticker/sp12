#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include "InputModule.hpp"

//todo: this is a placeholder for now, until i figure out backpacks etc
class Inventory
:	public InputModule
{
public:
	Inventory(Mob* a_pOwnedMob);
	virtual void Update(float a_DeltaT);
	//
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	//
private:
	//
};

#endif	INVENTORY_HPP