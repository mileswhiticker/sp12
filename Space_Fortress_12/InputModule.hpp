#ifndef INPUT_MODULE_HPP
#define INPUT_MODULE_HPP

#include <OIS\OISMouse.h>
#include <OIS\OISKeyboard.h>
#include <OGRE\OgreVector3.h>

class Mob;
class Client;
class Atom;

class InputModule
:	public OIS::KeyListener
,	public OIS::MouseListener
{
public:
	InputModule(Mob* a_pOwnedMob, Client* a_pOwnedClient);
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
protected:
	Mob* m_pOwnedMob;
	Client* m_pOwnedClient;
	//
	void SelectNewAtom(Atom* a_pNewAtom);
	void ClearSelectedAtom();
	Atom* m_pCurrentlyTargettedAtom;
	//
private:
	//
};

#endif INPUT_MODULE_HPP