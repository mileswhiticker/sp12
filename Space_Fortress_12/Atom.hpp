#ifndef ATOM_HPP
#define ATOM_HPP

#include <OGRE\OgreColourValue.h>
#include <vector>
#include <set>

class btBoxShape;
class btRigidBody;
class ObserverBuild;
struct CachedCube;
class Turf;
class InputModule;
class Component;
class Mob;
class Context;
namespace Ogre
{
	class SceneNode;
	class Entity;
};

class Atom
{
public:
	enum AtomType
	{
		UNKNOWN_ATOMTYPE = 0,
		//
		STRUCTURE,
		OBJECT,
		MOB,
		TURF,
		//
		MAX_ATOMTYPE,
	};
	enum AtomFlags
	{
		UNKNOWN_ATOMFLAG = 0,
		//
		BUILD_TURF,
		BUILD_STRUCTURE,
		//
		MAX_ATOMFLAG,
	};
	Atom(Ogre::Vector3 a_Pos, int a_Dir = 0);
	virtual ~Atom();
	//
	virtual void Update(float a_DeltaT);
	virtual void InstantiateAtom()=0;
	virtual bool ChangeDirection(int a_NewDir);
	//
	void SetFlashingColour(Ogre::ColourValue a_FlashColour);
	void StopFlashingColour();
	//
	std::vector<Atom*> m_AtomContents;
	void SetEntityVisible(bool a_Visible = true);
	//
	virtual bool Interact(Mob* a_pSourceMob, Context* a_pSourceContext, int a_InteractType = 3, Atom* a_pUsedAtom = NULL);
	//
	Ogre::Entity* m_pAtomEntity;
	btRigidBody* m_pRigidBody;
	btBoxShape* m_pCollisionShape;
	Ogre::SceneNode* m_pAtomEntitySceneNode;
	Ogre::SceneNode* m_pAtomRootSceneNode;
	CachedCube* m_pCachedCube;	//used to draw the collision shape, assuming it's a cube
	//
	virtual void BuildTurf(Turf* a_pTarget, bool a_Virtual = false);
	virtual void BuildStructure(Turf* a_pTarget, bool a_Virtual = false);
	virtual bool OnGravityChange();
	virtual void ResetEnvironment();
	//
	AtomType GetAtomType();
	int GetAtomFlags();
	int GetDirection();
	Turf* GetCurrentTurf();
	std::string GetAtomTextName();
	//
	virtual void Select(Component* a_pSourceComponent);
	virtual void DeSelect(Component* a_pSourceComponent);
	//
	int GetAtomUID();
	//
protected:
	AtomType m_MyAtomType;
	int m_Direction;
	bool m_UseRigidbodyPosition;
	std::string m_AtomTextName;
	//
	void InitCollisionShapeDebugDraw(Ogre::ColourValue a_ColourVal);
	bool m_UsesGravity;
	Turf* m_pCurrentTurf;		//for handling gravity
	int m_AtomFlags;
	const int m_AtomID;
	std::map<int, Atom*> m_Contents;
	//
	std::list<Component*> m_AllComponents;
	//
	std::string m_MaterialName;
	std::string m_SelectMaterialName;
	std::string m_VirtualSelectMaterialName;
	//
private:
	float m_ColourModulateLevel;
	float m_tLeftUpdateCell;
	int m_ModulateChangeDir;
	std::set<Component*> m_SelectingInputModules;
	//
	//std::list< std::pair<> > m_CustomInteractions;
};

#endif	ATOM_HPP