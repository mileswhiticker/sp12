#include "Turf.hpp"

#include "GravitySource.hpp"
#include "Structure.hpp"
#include "MapSuite.hpp"
#include "AtomManager.hpp"

#include <OGRE\OgreEntity.h>

Turf::TurfType Turf::GetTurfType()
{
	return m_MyTurfType;
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

bool Turf::IsBuildpoint()
{
	return m_IsBuildPoint;
}

void Turf::SetBuildpoint(bool a_IsBuildpoint)
{
	m_IsBuildPoint = a_IsBuildpoint;
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

void Turf::AddStructure(Structure* a_pNewStructure)
{
	m_StructureContents.push_back(a_pNewStructure);
}

//todo: multiple storage containers sorted by dir, quadrant & layer to make this function faster?
bool Turf::IsStructureBlocked(int a_Dir, int a_Quadrant, int a_Layer)
{
	for(auto it = m_Structures.begin(); it != m_Structures.end(); ++it)
	{
		if((*it)->GetDirection() == a_Dir)
			return true;
		if((*it)->GetStructureQuadrant() == a_Quadrant)
			return true;
		if((*it)->GetStructureLevel() == a_Layer)
			return true;
	}
	return false;
}

void Turf::Select(InputModule* a_pSourceInputModule)
{
	Atom::Select(a_pSourceInputModule);
	if(m_pAtomEntity)
	{
		if(m_IsBuildPoint)
			m_pAtomEntity->setMaterialName(m_VirtualSelectMaterialName);
		else
			m_pAtomEntity->setMaterialName(m_SelectMaterialName);
	}
	AtomManager::GetSingleton().SetTurfWantingUpdate(this);
}

void Turf::DeSelect(InputModule* a_pSourceInputModule)
{
	Atom::DeSelect(a_pSourceInputModule);
	if(m_pAtomEntity)
	{
		m_pAtomEntity->setMaterialName(m_MaterialName);
	}
	if(m_IsBuildPoint)
	{
		SetEntityVisible(false);
	}
	AtomManager::GetSingleton().AtomManager::GetSingleton().StopTurfWantingUpdate(this);
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
			a_pLeavingAtom->ForceEjectFromTurf();
			break;
		}
	}
}

bool Turf::OnGravityChange()
{
	//??
	return false;

	for(auto it = m_AtomContents.begin(); it != m_AtomContents.end(); ++it)
	{
		(*it)->OnGravityChange();
	}
	return true;
}

Structure* Turf::GetMountedStructure(int a_Dir, int a_Layer)
{
	Structure* pOut = NULL;
	return pOut;
}