#include "RCD_InputModule.hpp"
#include "Client.hpp"
#include "Application.hpp"
#include "MapSuite.hpp"
#include "RCD.hpp"

#include "CollisionDefines.h"
#include "BtOgreHelper.hpp"
#include "DebugDrawer.h"
#include "EffectManager.hpp"
#include "Cached.hpp"
#include "Turf.hpp"
#include "Events.hpp"
#include "Structure.hpp"
#include "OverlayPlating.hpp"
#include "UnderlayPlating.hpp"
#include "GravPlates.hpp"
//#include "LightFixture.hpp"

#include <Ogre\OgreCamera.h>
#include <BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h>
#include <BulletCollision\CollisionDispatch\btCollisionWorld.h>
#include <cegui\elements\CEGUIGUISheet.h>
#include <OGRE\OgreEntity.h>

#include "Humanoid.hpp"
#include "AtomManager.hpp"
#include <BulletDynamics\Dynamics\btRigidBody.h>
#include <OGRE\OgreSceneNode.h>

#include "num2string.h"

RCD_InputModule::RCD_InputModule(Mob* a_pOwnedMob, RCD* a_pOwnedRCD)
:	InputModule(a_pOwnedMob)
,	m_pOwnedRCD(a_pOwnedRCD)
,	m_pCurrentTargetBuildpoint(NULL)
{
	//create a test human
	//m_pTestHumanoid = (Humanoid*)AtomManager::GetSingleton().CreateMob(Mob::HUMANOID, Ogre::Vector3::ZERO, NULL, 0 | INSTANTIATE_IMMEDIATELY);
	
	//todo: split this up so it can be created/cleared when mobs pick up or drop the RCD (with appropriate client checks of course)
}

void RCD_InputModule::Update(float a_DeltaT)
{
	//interact raycasting has been moved to mob code
}

//Mob::RaycastForward has found a bunch of atoms we might want to interact with
//it passed them on to us and now its our problem - Mob::RaycastForward doesn't want to know if we succeeded or failed
void RCD_InputModule::TryInteract(std::vector<Atom*>& a_InteractAtoms)
{
	if(a_InteractAtoms.size())
	{
		Atom* pAtomToInteract = NULL;
		for(auto it = a_InteractAtoms.begin(); it != a_InteractAtoms.end(); ++it)
		{
			if(TryInteract(*it, false))
			{
				//we'll take any old atom if there's nothing else to offer
				if(!pAtomToInteract)
				{
					pAtomToInteract = *it;
					continue;
				}
				
				//otherwise, let's check if there is a closer one
				if((*it)->GetAtomType() == Atom::STRUCTURE)
				{
					//otherwise, we'd prefer structures over turfs
					if(pAtomToInteract->GetAtomType() == Atom::TURF)
					{
						pAtomToInteract = *it;
						break;
					}
				}
			}
		}

		TryInteract(pAtomToInteract);
	}
}

//Mob::RaycastForward has found an atoms we might want to interact with (the closest possible one to us, actually)
//if we can't interact with this atom, Mob::RaycastForward will just send us all possible interact atoms (see the other overload of TryInteract above)
bool RCD_InputModule::TryInteract(Atom* a_pInteractAtom, bool a_DoInteract)
{
	m_pLastInteractAtom = a_pInteractAtom;		//possibly unsafe, but if it's updated regularly should be fine

	if(a_pInteractAtom && a_pInteractAtom->GetAtomFlags() & (Atom::RCD_BUILD_TARGET | Atom::RCD_CAN_DESTROY) )
	{
		if(m_pCurrentTargetBuildpoint)
		{
			//if we have a virtual buildpoint, we're trying to build something

			//lets make sure the types match up, and that the target atom
			if(m_pCurrentTargetBuildpoint->GetAtomType() == a_pInteractAtom->GetAtomType() && a_pInteractAtom->GetAtomFlags() & Atom::RCD_BUILD_TARGET)
			{
				//todo:check atom subtypes here. this works for now, but might bring up weird behaviour later

				//it is! let's tell the player we can build our specified atom here
				if(a_DoInteract)
				{
					Ogre::Vector3 oldPos = m_pCurrentTargetBuildpoint->m_pAtomRootSceneNode->getPosition();
					Ogre::Vector3 newPos = a_pInteractAtom->m_pAtomRootSceneNode->getPosition();
					m_pCurrentTargetBuildpoint->m_pAtomRootSceneNode->setPosition(newPos);
					m_pCurrentTargetBuildpoint->SetEntityVisible();
					SelectNewAtom(m_pCurrentTargetBuildpoint);
				}

				return true;	//successful interaction
			}
		}
		else
		{
			//if we don't have a virtual buildpoint, we're trying to destroy something
			
			//lets make sure the types match up (we're either trying to destroy the target atom or haven't specified an atom type, in which case destroy everything
			//and also check that the target atom has the RCD_CAN_DESTROY flag
			if( (!m_pOwnedRCD->m_TargetAtomTypes || m_pOwnedRCD->m_TargetAtomTypes & a_pInteractAtom->GetAtomType()) )
			{
				if(a_pInteractAtom->GetAtomFlags() & Atom::RCD_CAN_DESTROY)
				{
					//it is! let's tell the player we can destroy this atom
					if(a_DoInteract)
						SelectNewAtom(a_pInteractAtom);

					return true;	//successful interaction
				}
			}
		}
	}
	else if(a_DoInteract)
	{
		if(m_pCurrentTargetBuildpoint)
		{
			m_pCurrentTargetBuildpoint->SetEntityVisible(false);
		}
		else
		{
			SelectNewAtom(NULL);
		}
	}
	
	return false;	//failed interaction
}

bool RCD_InputModule::keyPressed( const OIS::KeyEvent &arg )
{
	switch(arg.key)
	{
	/*case(OIS::KC_T):
		{
			IncrementTargetBuildOption();
			//m_pOwnedMob->m_pPossessingClient->m_pTopInfoBar->setText("RCD build mode: girders");
			return true;
		}*/
	case(OIS::KC_B):
		{
			m_pOwnedMob->m_pPossessingClient->m_pTopInfoBar->setText("RCD build mode: DECONSTRUCT");
			m_pOwnedRCD->m_TargetAtomTypes = NULL;
			m_pOwnedRCD->m_TargetAtomSubTypes = NULL;

			if(m_pCurrentTargetBuildpoint)
			{
				if(m_pCurrentTargetBuildpoint->GetAtomType() == Atom::TURF)
				{
					AtomManager::GetSingleton().StopTurfWantingUpdate((Turf*)m_pCurrentTargetBuildpoint);
				}
				delete m_pCurrentTargetBuildpoint;
				m_pCurrentTargetBuildpoint = NULL;
			}
			return true;
		}
	case(OIS::KC_1):
		{
			m_pOwnedMob->m_pPossessingClient->m_pTopInfoBar->setText("RCD build mode: GIRDER");
			//SetTargetStructure(Turf::GIRDER);
			SelectNewAtom(NULL);
			
			//clear out the old buildpoint, if one exists
			if(m_pCurrentTargetBuildpoint)
			{
				if(m_pCurrentTargetBuildpoint->GetAtomType() == Atom::TURF)
				{
					AtomManager::GetSingleton().StopTurfWantingUpdate((Turf*)m_pCurrentTargetBuildpoint);
				}
				delete m_pCurrentTargetBuildpoint;
				m_pCurrentTargetBuildpoint = NULL;
			}

			//create a girder build point
			m_pOwnedRCD->m_TargetAtomTypes = Atom::TURF;
			m_pOwnedRCD->m_TargetAtomSubTypes = Turf::GIRDER;

			m_pCurrentTargetBuildpoint = new Turf( Ogre::Vector3(0,0,0) );
			((Turf*)m_pCurrentTargetBuildpoint)->Instantiate(Turf::GIRDER_VIRTUAL);
			AtomManager::GetSingleton().SetTurfWantingUpdate((Turf*)m_pCurrentTargetBuildpoint);

			return true;
		}
	case(OIS::KC_2):
		{
			//m_pOwnedMob->m_pPossessingClient->m_pTopInfoBar->setText("RCD build mode: OVERLAYPLATING");
			//SetTargetStructure(Structure::OVERLAYPLATING);
			m_pOwnedMob->SendClientMessage("Placement of overlay plates disabled for now!", 3);

			/*m_pOwnedMob->SendClientMessage("Placement of overlay plates is untested and probably broken.", 4);
			SelectNewAtom(NULL);
			
			//clear out the old buildpoint, if one exists
			if(m_pCurrentTargetBuildpoint)
			{
				delete m_pCurrentTargetBuildpoint;
				m_pCurrentTargetBuildpoint = NULL;
			}

			//create an overlayplating build point
			m_pOwnedRCD->m_TargetAtomTypes = Atom::STRUCTURE;
			m_pOwnedRCD->m_TargetAtomSubTypes = Structure::OVERLAYPLATING;

			m_pCurrentTargetBuildpoint = new OverlayPlating(m_pOwnedMob->GetCurrentTurf());
			((Turf*)m_pCurrentTargetBuildpoint)->Instantiate(Turf::GIRDER_VIRTUAL);*/

			return true;
		}
	case(OIS::KC_3):
		{
			//m_pOwnedMob->m_pPossessingClient->m_pTopInfoBar->setText("RCD build mode: UNDERLAYPLATING");
			//SetTargetStructure(Structure::UNDERLAYPLATING);
			m_pOwnedMob->SendClientMessage("Placement of underlay plates disabled for now!", 3);

			/*m_pOwnedMob->SendClientMessage("Placement of underlay plates is untested and probably broken.", 4);
			SelectNewAtom(NULL);
			
			//clear out the old buildpoint, if one exists
			if(m_pCurrentTargetBuildpoint)
			{
				delete m_pCurrentTargetBuildpoint;
				m_pCurrentTargetBuildpoint = NULL;
			}

			//create an overlayplating build point
			m_pOwnedRCD->m_TargetAtomTypes = Atom::STRUCTURE;
			m_pOwnedRCD->m_TargetAtomSubTypes = Structure::OVERLAYPLATING;

			m_pCurrentTargetBuildpoint = new OverlayPlating(m_pOwnedMob->GetCurrentTurf());
			((Turf*)m_pCurrentTargetBuildpoint)->Instantiate(Turf::GIRDER_VIRTUAL);*/

			return true;
		}
	case(OIS::KC_4):
		{
			//m_pOwnedMob->m_pPossessingClient->m_pTopInfoBar->setText("RCD build mode: LIGHTFIXTURE");
			//SetTargetStructure(Structure::LIGHTFIXTURE);
			m_pOwnedMob->SendClientMessage("Placement of light fixtures disabled for now!", 3);
			return true;
		}
	case(OIS::KC_5):
		{
			//m_pOwnedMob->m_pPossessingClient->m_pTopInfoBar->setText("RCD build mode: GRAVPLATES)");
			//SetTargetStructure(Structure::GRAVPLATES);
			m_pOwnedMob->SendClientMessage("Placement of grav plates disabled for now!", 3);
			return true;
		}
	/*case(OIS::KC_I):
		{
			if(m_pCurrentlyTargettedAtom && m_pCurrentlyTargettedAtom->GetAtomType() == Atom::STRUCTURE)
			{
				if(((Structure*)m_pCurrentlyTargettedAtom)->IsBuildPoint())
					((Structure*)m_pCurrentlyTargettedAtom)->CreateFromBuildPoint();
				else
					((Structure*)m_pCurrentlyTargettedAtom)->DestroyToBuildPoint();
			}
			return true;
		}*/
	/*case(OIS::KC_U):
		{
			MapSuite::SaveMapDefaultName();
		}*/
	}
	
	return false;
}

bool RCD_InputModule::mouseMoved( const OIS::MouseEvent &arg )
{
	//zmovement represents mousewheel
	//each 120 units represents a 'notch'
	if (arg.state.Z.rel)
	{
		//m_pOwnedRCD->CycleBuildRange(m_pOwnedMob, arg.state.Z.rel / 120);
		
		//virtual void CycleBuildRange(Mob* a_pTriggeringMob, int a_Amount = 1);
		m_pOwnedRCD->m_CellBuildRange += arg.state.Z.rel / 120;
		if(m_pOwnedRCD->m_CellBuildRange < 0)
			m_pOwnedRCD->m_CellBuildRange = 0;
		else if(m_pOwnedRCD->m_CellBuildRange > 6)
			m_pOwnedRCD->m_CellBuildRange = 6;
		m_pOwnedMob->SendClientMessage("RCD build range set to " + num2string(m_pOwnedRCD->m_CellBuildRange), 1);
	}
	return false;
}

bool RCD_InputModule::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if(id == OIS::MB_Left)
	{
		if(m_pCurrentTargetBuildpoint)
		{
			//create the buildpoint
			if(m_pOwnedRCD->m_TargetAtomTypes == Atom::TURF)
			{
				Turf* pVirtualTurf = (Turf*)m_pCurrentTargetBuildpoint;
				if(m_pLastInteractAtom && m_pLastInteractAtom->GetAtomType() == Atom::TURF)
				{
					MapSuite::GetSingleton().BuildTurf((Turf*)m_pLastInteractAtom, m_pOwnedRCD->m_TargetAtomSubTypes);
				}
				else if(m_pLastInteractAtom)
				{
					m_pOwnedMob->SendClientMessage("RCD_InputModule: Last interacted atom type does not match build atom type!", 3);
				}
			}
			else
			{
				//create structures
			}
		}
		else if(m_pLastInteractAtom)
		{
			//destroy the targetted atom

			if(m_pLastInteractAtom->GetAtomFlags() & Atom::RCD_CAN_DESTROY)
			{
				if(m_pLastInteractAtom->GetAtomType() == Atom::TURF)
				{
					AtomManager::GetSingleton().StopTurfWantingUpdate((Turf*)m_pLastInteractAtom);
					MapSuite::GetSingleton().DeleteTurf((Turf*)m_pLastInteractAtom);
				}
			}
			else
			{
				m_pOwnedMob->SendClientMessage("RCD_InputModule: Last interacted atom type does not have RCD_CAN_DESTROY flag!", 3);
			}
		}
	}

	//raycast forward and loop over contexts until a valid one is found
	/*if(m_pCurrentlyTargettedAtom && m_pOwnedMob)
	{
		m_pCurrentlyTargettedAtom->Interact(m_pOwnedMob, this, m_pOwnedMob->GetIntent(), Event::TOGGLEBUILD);
		return true;
	}*/
	return false;
}

//todo: is this needed?
bool RCD_InputModule::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return false;
}

/*void RCD_InputModule::ToggleBuildExpansion()
{
	if(m_pOwnedMob)
	{
		m_pOwnedRCD->m_BuildExpansion = !m_pOwnedRCD->m_BuildExpansion;
		std::string message;
		if(m_pOwnedRCD->m_BuildExpansion)
		{
			message = "RCD targetting buildpoints";
		}
		else
		{
			message = "RCD targetting existing structures";
		}
		m_pOwnedMob->SendClientMessage(message, 1);
	}
}*/

void RCD_InputModule::SetTargetStructure(int a_StructureType)
{
	ClearBuildTarget();

	m_pOwnedRCD->m_TargetAtomTypes = Atom::STRUCTURE;
	m_pOwnedRCD->m_TargetAtomSubTypes = a_StructureType;
	m_pOwnedMob->SendClientMessage("RCD targetting structure type " + num2string(a_StructureType), 1);
}

void RCD_InputModule::SetTargetTurf(int a_TurfType)
{
	ClearBuildTarget();

	m_pOwnedRCD->m_TargetAtomTypes = Atom::TURF;
	m_pOwnedRCD->m_TargetAtomSubTypes = a_TurfType;
	m_pOwnedMob->SendClientMessage("RCD targetting turf type " + num2string(a_TurfType), 1);
}

void RCD_InputModule::ClearBuildTarget()
{
	if(m_pCurrentTargetBuildpoint)
	{
		bool success = false;
		if(m_pCurrentTargetBuildpoint->GetAtomType() == Atom::TURF)
		{
			Turf* pTurf= (Turf*)m_pCurrentTargetBuildpoint;						//HAHAHAHAHAHAHAHA
			MapSuite::GetSingleton().DeleteTurf(pTurf);
			success = true;
		}
		else if(m_pCurrentTargetBuildpoint->GetAtomType() == Atom::STRUCTURE)
		{
			Structure* pStructure = (Structure*)m_pCurrentTargetBuildpoint;		//oh god won't somebody think of the children?
			AtomManager::GetSingleton().DeleteStructure(pStructure);
			success = true;
		}

		if(success)
		{
			m_pCurrentTargetBuildpoint = NULL;
		}
	}
}

/*void RCD_InputModule::IncrementTargetBuildOption(int a_IndexIncrement)
{
	int numBuildOptions = m_pOwnedRCD->m_BuildOptions.size();
	if(numBuildOptions)
	{
		m_pOwnedRCD->m_BuildOptionsIndex += a_IndexIncrement;
		while(m_pOwnedRCD->m_BuildOptionsIndex >= numBuildOptions)
		{
			m_pOwnedRCD->m_BuildOptionsIndex -= numBuildOptions;
		}

		//hide previous ghost if it's visible
		if(m_pCurrentTargetBuildpoint)
		{
			m_pCurrentTargetBuildpoint->SetEntityVisible(false);
		}

		m_pCurrentTargetBuildpoint = m_pOwnedRCD->m_BuildOptions[m_pOwnedRCD->m_BuildOptionsIndex];
		m_pCurrentTargetBuildpoint->SetEntityVisible(true);
	}
}*/

float RCD_InputModule::GetInteractRange()
{
	return m_pOwnedRCD->m_CellBuildRange;
}

int RCD_InputModule::GetInteractFilterGroup()
{
	return RAYCAST_BUILD;
}

int RCD_InputModule::GetInteractFilterMask()
{
	if(m_pOwnedRCD->m_TargetAtomTypes == Atom::STRUCTURE)
		return COLLISION_STRUCTURE;
	else if(m_pOwnedRCD->m_TargetAtomTypes == Atom::TURF)
		return COLLISION_TURF;

	return COLLISION_TURF|COLLISION_STRUCTURE;
}

int RCD_InputModule::GetInteractAtomType()
{
	return m_pOwnedRCD->m_TargetAtomTypes;
}

int RCD_InputModule::GetInteractAtomSubtype()
{
	return m_pOwnedRCD->m_TargetAtomSubTypes;
}
