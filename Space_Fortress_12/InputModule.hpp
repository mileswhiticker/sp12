#ifndef INPUTMODULE_HPP
#define INPUTMODULE_HPP

#include <OIS\OISMouse.h>
#include <OIS\OISKeyboard.h>
#include <OGRE\OgreVector3.h>

class Mob;
class Client;
class Atom;
class btRigidBody;

class InputModule
:	public OIS::KeyListener
,	public OIS::MouseListener
{
public:
	InputModule(Mob* a_pOwnedMob);
	virtual void Update(float a_DeltaT)=0;
	//The mob must learn about this input module separately
	void SetMobOwner(Mob* a_pMob);
	//
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	//
	void ForceClearAtomIfSelected(Atom* a_pOtherAtom);
	//
	//void CheckInteraction(Atom* a_pSourceAtom, Atom* a_pMethodAtom);
	virtual float GetInteractRange();
	virtual int GetInteractFilterGroup();
	virtual int GetInteractFilterMask();
	virtual int GetInteractAtomType();
	virtual int GetInteractAtomSubtype();

	// virtual abstract functions?
	virtual bool TryInteract(Atom* a_pInteractAtom, bool a_DoInteract = true);
	virtual void TryInteract(std::vector<Atom*>& a_InteractAtoms);
	//
protected:
	Mob* m_pOwnedMob;
	//
	void SelectNewAtom(Atom* a_pNewAtom);
	void ClearSelectedAtom();
	Atom* m_pCurrentlyTargettedAtom;
	std::vector<btRigidBody*> m_InteractBodies;
	Atom* m_pLastInteractAtom;
	//
};

#endif INPUTMODULE_HPP