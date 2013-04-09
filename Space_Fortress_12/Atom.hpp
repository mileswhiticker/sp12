#ifndef ATOM_HPP
#define ATOM_HPP

#include <OGRE\OgreColourValue.h>
#include <vector>
#include <set>

class btBoxShape;
class btRigidBody;
class ObserverBuild;
struct CachedCube;
class MapCell;
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
		UNKNOWN = 0,
		STRUCTURE,
		OBJECT,
		MOB,
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
	Ogre::Entity* m_pAtomEntity;
	btRigidBody* m_pRigidBody;
	btBoxShape* m_pCollisionShape;
	Ogre::SceneNode* m_pAtomEntitySceneNode;
	Ogre::SceneNode* m_pAtomRootSceneNode;
	CachedCube* m_pCachedCube;	//used to draw the collision shape, assuming it's a cube
	//
	AtomType GetAtomType();
	int GetDirection();
	//
	virtual void Select(ObserverBuild* a_pSelectingObserver);
	virtual void DeSelect(ObserverBuild* a_pSelectingObserver);
	//
protected:
	AtomType m_MyAtomType;
	int m_Direction;
	bool m_UseRigidbodyPosition;
	//
	void InitCollisionShapeDebugDraw(Ogre::ColourValue a_ColourVal);
	bool m_UsesGravity;
	MapCell* m_pSourceMapCell;		//for handling gravity
	//
private:
	float m_ColourModulateLevel;
	int m_ModulateChangeDir;
	std::set<ObserverBuild*> m_pSelectingObservers;
	//
};

#endif	ATOM_HPP