#ifndef GIRDER_HPP
#define GIRDER_HPP

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
	virtual void InstantiateStructure(bool a_IsBuildPoint);
	virtual void CreateFromBuildPoint();
	virtual void DestroyToBuildPoint();
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
	MapCell* m_pSourceMapCell;
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