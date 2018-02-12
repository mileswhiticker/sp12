#include "RCD.hpp"
#include "Mob.hpp"
#include "RCD_InputModule.hpp"

#include "GirderBuild.hpp"
#include "AtomManager.hpp"

RCD::RCD(Ogre::Vector3 a_StartPos, int a_StartDirection)
:	Object(a_StartPos, a_StartDirection)
,	m_CellBuildRange(1)
//,	m_BuildExpansion(true)
,	m_TargetAtomTypes(0)
,	m_TargetAtomSubTypes(0)
//,	m_BuildOptionsIndex(0)
{
	m_AtomTextName = "Remote Construction Device";
	m_pEquipInputModule = new RCD_InputModule(NULL, this);

	//m_BuildOptions.push_back(new ());
}
