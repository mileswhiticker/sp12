#ifndef TURF_HPP
#define TURF_HPP

#include "Atom.hpp"
#include "Context.hpp"

#include <unordered_map>
#include <OGRE\OgreVector3.h>

class InputModule;
class Structure;
class GravitySource;
struct Buildpoint;
class Mob;
class Object;

class Turf
:	public Atom
{
public:
	enum TurfType
	{
		TURF_UNKNOWN = 0,
		//
		BUILDTURF,
		GIRDER,
		GIRDER_VIRTUAL,
		//
		TURF_MAXTYPE
	};
	enum StructureLevel
	{
		UNKNOWNLEVEL = 0,
		//
		INNER,
		UNDER,
		CONDUIT,
		OVER,
		OUTER,
		//
		MAX_LEVEL
	};
	Turf(Ogre::Vector3 a_Location);
	~Turf();
	//
	virtual void InstantiateAtom();
	//virtual void CreateFromBuildPoint();
	//virtual void DestroyToBuildPoint();
	//
	TurfType GetTurfType();
	//Structure* m_pTurfStructure;
	Buildpoint* GetBuildpointOrNull(int a_Dir, int a_Position);
	void AddStructure(Structure* a_pNewStructure);
	//
	virtual bool Interact(Mob* a_pSourceMob, Context* a_pSourceContext, int a_InteractType = 3, Atom* a_pUsedAtom = NULL);
	virtual void Select(InputModule* a_pSourceInputModule);
	virtual void DeSelect(InputModule* a_pSourceInputModule);
	//
	bool IsBuildpoint();
	void SetBuildpoint(bool a_IsBuildpoint = true);
	//
	void AddGravityAffector(GravitySource* a_pGravSource, float a_DistScalar);
	void RemoveGravityAffector(GravitySource* a_pGravSource);
	Ogre::Vector3 GetGravity();
	//
	void AtomLeave(Atom* a_pLeavingAtom);
	void AtomEnter(Atom* a_pEnteringAtom);
	virtual bool OnGravityChange();
	//
	Structure* GetMountedStructure(int a_Dir, int a_Layer);
	//
	std::vector<Mob*> m_MobContents;
	std::vector<Atom*> m_AtomContents;
	std::vector<Object*> m_ObjContent;
	std::vector<Structure*> m_StructureContents;
	//
	void Instantiate(Turf::TurfType a_NewTurfType, Ogre::Entity* a_pCachedEntity = NULL);
	virtual bool AttemptInteract(Mob* a_pSourceMob, Atom* a_pUsedAtom, int a_InteractType);
	//
	std::vector<Atom*> m_AtomsWantingGravUpdates;
	Ogre::SceneNode* m_pCellSceneNode;
private:
	std::vector< std::pair<GravitySource*,float> > m_GravityAffectors;
	std::string m_CoordsString;
	//
protected:
	TurfType m_MyTurfType;
	bool m_IsBuildPoint;
	std::unordered_map<std::string, Buildpoint*> m_Buildpoints;							//unused
	
	//instead of using these two complex sorted containers, just use a simple list - we don't need to access any of them directly
	std::list<Structure*> m_Structures;
	Structure* m_pTargetBuildpoint;			//what the interacting player wants to build
	bool IsStructureBlocked(int a_Dir, int a_Quadrant, int a_Layer);
	/*
	std::unordered_map< int, std::vector<Structure*> > m_StructurePoints_DirLayer;		//remember to add 0 index here, or else handle it elsewhere
	std::vector< std::unordered_map<int, Structure*> > m_StructurePoints_LayerDir;		//unfilled, not sure if it's needed?
	*/
	//
	void InstantiateGirder(bool a_IsBuildPoint);
	//
	bool m_IsBuildable;
	//
};

#endif	TURF_HPP