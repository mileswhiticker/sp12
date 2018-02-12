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
class InputModule;
class Mob;
class Context;
class Object;

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
		RCD_BUILD_TARGET = 1,
		RCD_CAN_DESTROY = 2,
		//
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
	int m_DefaultPhysicsGroup;
	int m_DefaultPhysicsMask;
	//
	virtual void ForceEjectFromTurf();
	virtual bool OnGravityChange();
	virtual void ResetEnvironment();
	//
	AtomType GetAtomType();
	int GetAtomFlags();
	int GetDirection();
	Turf* GetCurrentTurf();
	std::string GetAtomTextName();
	//
	virtual void Select(InputModule* a_pSourceInputModule);
	virtual void DeSelect(InputModule* a_pSourceInputModule);
	//
	virtual bool AddAtomToContents(Atom* a_pAtom);
	virtual bool RemoveAtomFromContents(Atom* a_pAtom);
	//
	virtual bool AddToOtherAtomContents(Atom* a_pAtom);
	virtual bool RemoveFromOtherAtomContents();
	//
	int GetAtomUID();
	//
protected:
	AtomType m_MyAtomType;
	int m_Direction;
	bool m_UseRigidbodyPosition;
	bool m_UpdateTurfLoc;
	std::string m_AtomTextName;
	//
	void InitCollisionShapeDebugDraw(Ogre::ColourValue a_ColourVal);
	bool m_UsesGravity;
	Turf* m_pCurrentTurf;		//for handling gravity
	int m_AtomFlags;
	const int m_AtomID;

	//interaction
	Context* m_pMyContext;

	//contents
	std::map<int, Atom*> m_Contents;
	Atom* m_pHoldingAtom;

	//rendering
	std::string m_MaterialName;
	std::string m_SelectMaterialName;
	std::string m_VirtualSelectMaterialName;
	//
private:
	float m_ColourModulateLevel;
	float m_tLeftUpdateCell;
	int m_ModulateChangeDir;
	std::set<InputModule*> m_SelectingInputModules;
	//
	//std::list< std::pair<> > m_CustomInteractions;
};

#endif	ATOM_HPP