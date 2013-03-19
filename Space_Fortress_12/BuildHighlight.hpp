#ifndef BUILD_HIGHLIGHT_HPP
#define BUILD_HIGHLIGHT_HPP

#include <OGRE\OgreVector3.h>
#include "Atom.hpp"

class Obj;
class btRigidBody;
class btBoxShape;
namespace Ogre
{
	class SceneNode;
	class Entity;
}

class BuildHighlight
:	public Atom
{
public:
	BuildHighlight(Ogre::Vector3 a_Pos);
	virtual void Instantiate();
	//virtual void Update(float a_DeltaT);
};

#endif	BUILD_HIGHLIGHT_HPP