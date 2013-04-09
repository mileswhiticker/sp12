#include "InputModule_MobWalk.hpp"

MobWalk::MobWalk(Mob* a_pOwnedMob, Client* a_pOwnedClient)
:	InputModule(a_pOwnedMob, a_pOwnedClient)
{
	//
}

void MobWalk::Update(float a_DeltaT)
{
	//
}

bool MobWalk::keyPressed( const OIS::KeyEvent &arg )
{
	return true;
}

bool MobWalk::keyReleased( const OIS::KeyEvent &arg )
{
	switch(arg.key)
	{
	case(OIS::KC_W):
		{
			break;
		}
	case(OIS::KC_A):
		{
			break;
		}
	case(OIS::KC_S):
		{
			break;
		}
	case(OIS::KC_D):
		{
			break;
		}
	}
	return true;
}
