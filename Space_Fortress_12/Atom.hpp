#ifndef ATOM_HPP
#define ATOM_HPP

#include <OGRE\OgreColourValue.h>
#include <vector>

class btCollisionShape;
class btRigidBody;

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
		UNKNOWNTYPE = 0,
		OBJECT,
		GIRDER,
		BUILDHIGHLIGHT,
		//
	};
	Atom();
	//
	Ogre::SceneNode* m_pAtomSceneNode;
	virtual void Update(float a_DeltaT);
	virtual void Instantiate()=0;
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
	//
protected:
	AtomType m_MyAtomType;
	//
private:
	float m_ColourModulateLevel;
	int m_ModulateChangeDir;
	//
};

#endif	ATOM_HPP