#include "Box.hpp"

Box::Box(Ogre::Vector3 a_Position)
:	Object(a_Position, 0)
{
	//scenenode
	m_MyObjType = Object::BOX;
	m_MeshName = "cell_filling.mesh";
	m_AtomTextName = "Box";
}
