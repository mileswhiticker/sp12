#include "Turf.hpp"
#include "Events.hpp"
#include "Buildpoint.hpp"
//#include "MapCell.hpp"
#include "GravitySource.hpp"
#include "Mob.hpp"

#include <OGRE\OgreSceneNode.h>
#include <ogre\OgreEntity.h>

#include "OgreHelper.hpp"
#include "num2string.h"

Turf::Turf(Ogre::Vector3 a_Location)
:	Atom(a_Location)
,	Context(this, this)		//safe in this instance, not good general practice
//,	m_pTurfStructure(0)
,	m_MyTurfType(TURF_UNKNOWN)
,	m_IsBuildPoint(true)
,	m_IsBuildable(false)
{
	m_MyAtomType = Atom::TURF;
	m_MyTurfType = Turf::TURF_UNKNOWN;
	m_pAtomRootSceneNode->setPosition(a_Location);
	
	//initialise context stuff here, instead of in Context::Context()
	m_ContextAtomType = m_MyAtomType;
	m_ContextAtomSubType = m_MyTurfType;
	m_MyContextType = Context::BUILD;

	for(int curDir = 1; curDir <= 32; curDir *= 2)
	{
		break;
		for(int curPos = Buildpoint::INNER_PLATING_OUT; curPos <= Buildpoint::POS_MAXVAL; curPos += 1)
		{
			std::stringstream out;
			out << curDir << curPos;
			m_Buildpoints.insert( std::pair<std::string, Buildpoint*>(out.str(), new Buildpoint(*this, curPos, curDir, false) ) );
		}
	}
}

void Turf::Select(Component* a_pSourceComponent)
{
	Atom::Select(a_pSourceComponent);
	if(m_pAtomEntity)
	{
		if(m_IsBuildPoint)
			m_pAtomEntity->setMaterialName(m_VirtualSelectMaterialName);
		else
			m_pAtomEntity->setMaterialName(m_SelectMaterialName);
	}
}

void Turf::DeSelect(Component* a_pSourceComponent)
{
	Atom::DeSelect(a_pSourceComponent);
	if(m_IsBuildPoint)
	{
		SetEntityVisible(false);
	}
	if(m_pAtomEntity)
	{
		m_pAtomEntity->setMaterialName(m_MaterialName);
	}
}

Turf::TurfType Turf::GetTurfType()
{
	return m_MyTurfType;
}

bool Turf::Interact(Mob* a_pSourceMob, Context* a_pSourceContext, int a_InteractType, Atom* a_pUsedAtom)
{
	switch(a_pSourceContext->GetContextType())
	{
	case(Context::BUILD):
		{
			if(!m_MyTurfType)
			{
				Instantiate(Turf::GIRDER);
				return true;
			}
			return false;
		}
	case(Context::UNBUILD):
		{
			if(m_MyTurfType)
			{
				Instantiate(Turf::EMPTY);
				return true;
			}
			return false;
		}
	}

	//just pass it up the chain
	return Atom::Interact(a_pSourceMob, a_pSourceContext, a_InteractType, a_pUsedAtom);
}

bool Turf::IsBuildPoint()
{
	return m_IsBuildPoint;
}

Buildpoint* Turf::GetBuildpointOrNull(int a_Dir, int a_Position)
{
	std::stringstream index;
	index << a_Dir << a_Position;
	Buildpoint* pReturnVal = NULL;
	try
	{
		pReturnVal = m_Buildpoints.at(index.str());
	}
	catch (const std::out_of_range& oor)
	{
		//nothing
	}
	return pReturnVal;
}

bool Turf::IsBuildable()
{
	return m_IsBuildable;
}

void Turf::SetBuildable(bool a_CanBuild)
{
	m_IsBuildable = a_CanBuild;
}

Ogre::Vector3 Turf::GetGravity()
{
	Ogre::Vector3 out = Ogre::Vector3::ZERO;
	for(auto it = m_GravityAffectors.begin(); it != m_GravityAffectors.end(); ++it)
	{
		out += it->first->GetGravity() * it->second;
	}
	return out;
}

void Turf::AddGravityAffector(GravitySource* a_pGravSource, float a_DistScalar)
{
	m_GravityAffectors.push_back( std::pair<GravitySource*,float>(a_pGravSource, a_DistScalar) );
	OnGravityChange();
}

void Turf::RemoveGravityAffector(GravitySource* a_pGravSource)
{
	for(auto it = m_GravityAffectors.begin(); it != m_GravityAffectors.end(); ++it)
	{
		if(it->first == a_pGravSource)
		{
			m_GravityAffectors.erase(it);
			break;
		}
	}
	OnGravityChange();
}

void Turf::AtomEnter(Atom* a_pEnteringAtom)
{
	m_AtomContents.push_back(a_pEnteringAtom);
	a_pEnteringAtom->ResetEnvironment();
}

void Turf::AtomLeave(Atom* a_pLeavingAtom)
{
	for(auto it = m_AtomContents.begin(); it != m_AtomContents.end(); ++it)
	{
		if(*it == a_pLeavingAtom)
		{
			m_AtomContents.erase(it);
			break;
		}
	}
}

bool Turf::OnGravityChange()
{
	for(auto it = m_AtomContents.begin(); it != m_AtomContents.end(); ++it)
	{
		(*it)->OnGravityChange();
	}
	return true;
}

//unused, would any atom need this?
void Turf::InstantiateAtom()
{
	Instantiate(Turf::EMPTY);
}

void Turf::Instantiate(Turf::TurfType a_NewTurfType, Ogre::Entity* a_pCachedEntity)
{
	if(a_NewTurfType != m_MyTurfType)
	{
		//clear out old entity if necessary
		if( m_pAtomEntity && (a_NewTurfType == Turf::EMPTY || a_pCachedEntity) )
		{
			Ogre::SceneNode* pSceneNode = m_pAtomEntity->getParentSceneNode();
			if(pSceneNode)
			{
				pSceneNode->detachObject(m_pAtomEntity);
			}
			delete m_pAtomEntity;
			m_pAtomEntity = NULL;
		}

		if(a_pCachedEntity)
		{
			m_pAtomEntity = a_pCachedEntity;
			m_pAtomEntitySceneNode->attachObject(m_pAtomEntity);
		}

		//custom stuff for each turf type (entity material)
		switch(a_NewTurfType)
		{
		case(Turf::GIRDER):
			{
				if(!m_pAtomEntity)
				{
					//a single cuboid girder to cover this cell
					Ogre::SceneManager& sceneManager = GetSceneManager();
					m_pAtomEntity = sceneManager.createEntity("girder_" + num2string(m_AtomID), "girder.mesh");
					m_pAtomEntitySceneNode->attachObject(m_pAtomEntity);
					StopFlashingColour();
				}

				if(m_pAtomEntity)
				{
					m_pAtomEntity->setMaterialName("girder_material");
				}
				break;
			}
		}
		m_MyTurfType = a_NewTurfType;
		m_ContextAtomSubType = m_MyTurfType;
	}
}

bool Turf::AttemptInteract(Mob* a_pSourceMob, Atom* a_pUsedAtom, int a_InteractType)
{
	std::cout << "DEBUG: Turf::AttemptInteract()" << std::endl;

	switch(a_InteractType)
	{
	case(Mob::TARGET):
		{
			if(m_pTargetEntity && m_pSceneNode && !m_pTargetEntity->getParentSceneNode())
			{
				m_pSceneNode->attachObject(m_pTargetEntity);
			}
			break;
		}
	case(Mob::UNTARGET):
		{
			if(m_pTargetEntity && m_pSceneNode && m_pTargetEntity->getParentSceneNode())
			{
				m_pSceneNode->detachObject(m_pTargetEntity);
			}
			break;
		}
	case(Mob::INTERACT):
		{
			Ogre::SceneNode* pSceneNode = m_pTargetEntity->getParentSceneNode();
			if(pSceneNode)
			{
				pSceneNode->detachObject(m_pTargetEntity);
			}
			break;
		}
	default:
		{
			return Context::AttemptInteract(a_pSourceMob, a_pUsedAtom, a_InteractType);
		}
	}

	return true;
}
