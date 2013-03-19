#ifndef GIRDER_HPP
#define GIRDER_HPP

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

class Girder
:	public Atom
{
public:
	Girder(Ogre::Vector3 a_Pos);
	virtual void Instantiate();
	//virtual void Update(float a_DeltaT);
	//
	bool AddOverlay(int a_Dir, std::string a_OverlayID);
	void ResetEmptyOverlays();
	bool AddUnderlay(int a_Dir, std::string a_UnderlayID);
	//bool AddFilling(std::string a_UnderlayID);
	void AddFreefloatingObj(std::string a_TypeTag);

	//todo: make overlays and underlays into atoms
	std::vector<btRigidBody*> m_RigidBodies;
	std::vector<btBoxShape*> m_BoxCollisionShapes;
	//
private:
	std::vector<Ogre::Entity*> m_QuarterGirderGirders;
	std::vector<Ogre::Entity*> m_Overlays;
	std::vector<Ogre::Entity*> m_Underlays;
	int m_PlateOverlayDirs;
	int m_PlateUnderlays;
	//
};

#endif	GIRDER_HPP