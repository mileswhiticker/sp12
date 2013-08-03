#ifndef CONTEXT_HPP
#define CONTEXT_HPP

class Turf;
class Mob;
class Atom;
//
class btRigidBody;
class btCollisionShape;

namespace Ogre
{
	class Entity;
	class SceneNode;
};

class Context
{
public:
	enum ContextType
	{
		CONTEXT_UNKNOWN = 0,
		//
		BUILD = 1,
		UNBUILD = 2,
		PICKUP = 4,
		BUTTONPRESS = 8,
		CARDSWIPE = 16,
		HANDSCAN,
		SIT,
		//
		CONTEXT_MAX
	};
	Context(Turf* a_pLocTurf, Atom* a_pOwnedAtom);
	
	virtual bool AttemptInteract(Mob* a_pSourceMob, Atom* a_pUsedAtom, int a_InteractType);
	//
	ContextType GetContextType();
	int GetAtomType();
	int GetAtomSubType();
	//
	void SetEnabled(bool a_Enabled = true);
	//
protected:
	btRigidBody* m_pRigidBody;
	btCollisionShape* m_pCollisionShape;
	bool m_Enabled;
	//
	Atom* m_pOwnedAtom;
	Turf* m_pLocTurf;
	//
	ContextType m_MyContextType;
	int m_ContextAtomType;
	int m_ContextAtomSubType;
	//
	Ogre::Entity* m_pTargetEntity;
	Ogre::SceneNode* m_pSceneNode;
};

#endif	CONTEXT_HPP