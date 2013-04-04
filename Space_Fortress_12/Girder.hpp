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
	virtual void CreateFromBuildPoint();
	virtual void DestroyToBuildPoint();
	//virtual void Update(float a_DeltaT);
	//
	//bool AddOverlay(int a_Dir, std::string a_OverlayID);
	//void ResetEmptyOverlays();
	//bool AddUnderlay(int a_Dir, std::string a_UnderlayID);
	//bool AddFilling(std::string a_UnderlayID);
	void AddFreefloatingObj(std::string a_TypeTag);

	MapCell* m_pSourceMapCell;
	//
private:
	std::list<Structure*> m_MountedStructures;
	std::list<Structure*> m_InvisibleBuildPoints;
	int m_PlateOverlayDirs;
	int m_PlateUnderlays;
	//
};

#endif	GIRDER_HPP