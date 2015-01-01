#include "Inventory.hpp"
#include "Mob.hpp"

Inventory::Inventory(Mob* a_pOwnedMob)
:	InputModule(a_pOwnedMob)
{
	//
}

void Inventory::Update(float a_DeltaT)
{
	//
}

bool Inventory::keyPressed( const OIS::KeyEvent &arg )
{
	switch(arg.key)
	{
	case(OIS::KC_I):
		{
			if(m_pOwnedMob)
			{
				/*for(auto it = m_pOwnedMob->m_Contents.begin(); it != m_pOwnedMob->m_Contents.end(); ++it)
				{
					//
				}*/
			}
			return true;
		}
	}
	
	return false;
}
