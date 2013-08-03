#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <OIS\OISMouse.h>
#include <OIS\OISKeyboard.h>
#include <OGRE\OgreVector3.h>

class Mob;
class Client;
class Atom;
class btRigidBody;

class Component
:	public OIS::KeyListener
,	public OIS::MouseListener
{
public:
	Component(Mob* a_pOwnedMob, Client* a_pOwnedClient);
	virtual void Update(float a_DeltaT)=0;
	void SetClient(Client* a_pClient);
	//
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	//
	void ForceClearAtomIfSelected(Atom* a_pOtherAtom);
	//
	void CheckInteraction(Atom* a_pSourceAtom, Atom* a_pMethodAtom);
	//
protected:
	Mob* m_pOwnedMob;
	Client* m_pOwnedClient;
	//
	void SelectNewAtom(Atom* a_pNewAtom);
	void ClearSelectedAtom();
	Atom* m_pCurrentlyTargettedAtom;
	std::vector<btRigidBody*> m_InteractBodies;
	//
private:
	//
};

#endif COMPONENT_HPP