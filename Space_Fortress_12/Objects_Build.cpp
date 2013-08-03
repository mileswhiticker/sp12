#include "Objects_Build.hpp"
#include "Turf.hpp"

#include "RandHelper.h"

#include <LinearMath\btVector3.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>

GirderObject::GirderObject(Ogre::Vector3 a_StartPos)
:	Object(a_StartPos)
,	m_pHighlightMesh(NULL)
,	m_pHighlightTurf(NULL)
,	m_HighlightDir(0)
,	m_HighlightLayer(0)
{
	m_MeshName = "cell_filling.mesh";
}

void GirderObject::Build(Turf* a_pLocTurf, int a_Direction, int a_Layer)
{
	//do stuff here
}

void GirderObject::BuildHighlight(Turf* a_pLocTurf, int a_Direction, int a_Layer)
{
	//do stuff here
}
