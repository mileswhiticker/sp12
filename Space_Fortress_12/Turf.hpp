#ifndef TURF_HPP
#define TURF_HPP

#include "Atom.hpp"
#include "Context.hpp"

#include <unordered_map>
#include <OGRE\OgreVector3.h>

class InputModule;
class Structure;
struct Buildpoint;
class GravitySource;
class Mob;
class Object;

class Turf
:	public Atom
,	public Context
{
public:
	enum TurfType
	{
		TURF_UNKNOWN = 0,
		//
		EMPTY,
		GIRDER,
		//
		TURF_MAXTYPE
	};
	Turf(Ogre::Vector3 a_Location);
	//
	virtual void InstantiateAtom();
	//virtual void CreateFromBuildPoint();
	//virtual void DestroyToBuildPoint();
	//
	TurfType GetTurfType();
	//Structure* m_pTurfStructure;
	bool IsBuildPoint();
	Buildpoint* GetBuildpointOrNull(int a_Dir, int a_Position);
	//
	virtual bool Interact(Mob* a_pSourceMob, Context* a_pSourceContext, int a_InteractType = 3, Atom* a_pUsedAtom = NULL);
	virtual void Select(Component* a_pSourceComponent);
	virtual void DeSelect(Component* a_pSourceComponent);
	//
	bool IsBuildable();
	void SetBuildable(bool a_CanBuild = true);
	//
	void AddGravityAffector(GravitySource* a_pGravSource, float a_DistScalar);
	void RemoveGravityAffector(GravitySource* a_pGravSource);
	Ogre::Vector3 GetGravity();
	//
	void AtomLeave(Atom* a_pLeavingAtom);
	void AtomEnter(Atom* a_pEnteringAtom);
	virtual bool OnGravityChange();
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
	//
protected:
	TurfType m_MyTurfType;
	bool m_IsBuildPoint;
	std::unordered_map<std::string, Buildpoint*> m_Buildpoints;
	//
	void InstantiateGirder(bool a_IsBuildPoint);
	//
	bool m_IsBuildable;
	//
};

#endif	TURF_HPP