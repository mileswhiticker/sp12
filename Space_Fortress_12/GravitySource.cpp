#include "GravitySource.hpp"

GravitySource::GravitySource()
:	m_Enabled(false)
,	m_Gravity(Ogre::Vector3::ZERO)
{
	//
}

Ogre::Vector3 GravitySource::GetGravity()
{
	return m_Gravity;
}
