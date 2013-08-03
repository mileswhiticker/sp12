#include "Context.hpp"
#include "Mob.hpp"

#include <BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h>

#include "BulletHelper.hpp"

Context::Context(Turf* a_pLocTurf, Atom* a_pOwnedAtom)
:	m_MyContextType(CONTEXT_UNKNOWN)
,	m_pOwnedAtom(a_pOwnedAtom)
,	m_pLocTurf(a_pLocTurf)
,	m_pRigidBody(0)
,	m_pCollisionShape(0)
,	m_ContextAtomType(0)
,	m_ContextAtomSubType(0)
,	m_pTargetEntity(0)
,	m_pSceneNode(0)
,	m_Enabled(false)
{
	//
}

Context::ContextType Context::GetContextType()
{
	return m_MyContextType;
}

int Context::GetAtomType()
{
	return m_ContextAtomType;
}

int Context::GetAtomSubType()
{
	return m_ContextAtomSubType;
}

bool Context::AttemptInteract(Mob* a_pSourceMob, Atom* a_pUsedAtom, int a_InteractType)
{
	std::cout << "DEBUG: Context::AttemptInteract()" << std::endl;
	if(m_pOwnedAtom)
	{
		return m_pOwnedAtom->Interact(a_pSourceMob, this, a_InteractType, a_pUsedAtom);
	}

	return false;
}

void Context::SetEnabled(bool a_Enabled)
{
	if(m_pRigidBody)
	{
		if(m_Enabled)
		{
			GetDynamicsWorld().removeRigidBody(m_pRigidBody);
			m_Enabled = false;
		}
		else
		{
			GetDynamicsWorld().addRigidBody(m_pRigidBody);
			m_Enabled = true;
		}
	}
}
