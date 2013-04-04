#ifndef ATOM_HPP
#define ATOM_HPP

#include <OGRE\OgreColourValue.h>
#include <vector>
#include <set>

class btCollisionShape;
class btRigidBody;
class ObserverBuild;
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
	Atom();
	virtual ~Atom();
	//
	Ogre::SceneNode* m_pAtomSceneNode;
	virtual void Update(float a_DeltaT);
	virtual void InstantiateAtom()=0;
	virtual bool ChangeDirection(int a_NewDir);
	//
	void SetFlashingColour(Ogre::ColourValue a_FlashColour);
	void StopFlashingColour();
	//
	std::vector<Atom*> m_AtomContents;
	void SetEntityVisible(bool a_Visible = true);
	Ogre::Entity* m_pAtomEntity;
	//
	btRigidBody* m_pRigidBody;
	btCollisionShape* m_pCollisionShape;
	//
	AtomType GetAtomType();
	int GetDirection();
	//
	void Select(ObserverBuild* a_pSelectingObserver);
	void DeSelect(ObserverBuild* a_pSelectingObserver);
	//
	virtual void TargetcastIntercept();
	//
protected:
	AtomType m_MyAtomType;
	int m_Direction;
	bool m_UseRigidbodyPosition;
	//
private:
	float m_ColourModulateLevel;
	int m_ModulateChangeDir;
	std::set<ObserverBuild*> m_pSelectingObservers;
	//
};

#endif	ATOM_HPP