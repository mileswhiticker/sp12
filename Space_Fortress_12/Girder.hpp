#ifndef GIRDER_HPP
#define GIRDER_HPP

#include <list>
#include <OGRE\OgreVector3.h>

#include "Structure.hpp"
#include "Turf.hpp"

class Obj;
class btRigidBody;
class btBoxShape;
namespace Ogre
{
	class SceneNode;
	class Entity;
}
class MapCell;

class Girder
:	public Structure
,	public Turf
{
public:
	Girder(MapCell* a_pSourceMapCell);
	virtual ~Girder();
	virtual void InstantiateStructure(bool a_IsBuildPoint);
	//
	virtual void CreateFromBuildPoint();
	virtual void DestroyToBuildPoint();
	void CreateBuildpointInDir(Structure::StructureType a_BuildPointType, int a_Dir);
	//
	void AddFreefloatingObj(std::string a_TypeTag);
	//
	virtual void Select(ObserverBuild* a_pSelectingObserver);
	virtual void DeSelect(ObserverBuild* a_pSelectingObserver);

	//
private:
	std::list<Structure*> m_MountedStructures;
	std::list<Structure*> m_InvisibleBuildPoints;
	int m_PlateOverlayDirs;
	int m_PlateUnderlays;
	//
};

#endif	GIRDER_HPP