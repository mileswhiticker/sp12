#ifndef GIRDER_HPP
#define GIRDER_HPP

#include <list>
#include <OGRE\OgreVector3.h>

#include "Structure.hpp"
#include "Turf.hpp"

class Obj;
class btRigidBody;
class btBoxShape;
class InputModule;
namespace Ogre
{
	class SceneNode;
	class Entity;
}
class MapCell;

class Girder
:	public Turf
{
public:
	Girder(MapCell* a_pSourceMapCell);
	virtual ~Girder();
	//
	virtual void Update(float a_DeltaT);
	virtual void InstantiateTurf(bool a_IsBuildPoint);
	virtual void CreateFromBuildPoint();
	virtual void DestroyToBuildPoint();
	//
	//virtual void CreateFromBuildPoint();
	//virtual void DestroyToBuildPoint();
	void CreateBuildpointInDir(Structure::StructureType a_BuildPointType, int a_Dir);
	//
	void AddFreefloatingObj(std::string a_TypeTag);
	virtual void Interact(Atom* a_pSourceAtom, InputModule* a_pSourceModule, int a_Intent, int a_Type = 0);
	//
	virtual void Select(InputModule* a_pSelectingInputModule);
	virtual void DeSelect(InputModule* a_pSelectingInputModule);
	//
private:
	std::list<Structure*> m_MountedStructures;
	std::list<Structure*> m_InvisibleBuildPoints;
	int m_PlateOverlayDirs;
	int m_PlateUnderlays;
	//
};

#endif	GIRDER_HPP