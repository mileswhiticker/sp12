#ifndef InputModule_HPP
#define InputModule_HPP

#include <OIS\OISMouse.h>
#include <OIS\OISKeyboard.h>
#include <OGRE\OgreVector3.h>

class Mob;
class Client;

class InputModule
:	public OIS::KeyListener
,	public OIS::MouseListener
{
public:
	InputModule(Mob* a_pOwnedMob, Client* a_pOwnedClient);
	virtual void Update(float a_DeltaT)=0;
	void SetClient(Client* a_pClient);
	//
	virtual bool keyPressed( const OIS::KeyEvent &arg )=0;
	virtual bool keyReleased( const OIS::KeyEvent &arg )=0;
	virtual bool mouseMoved( const OIS::MouseEvent &arg )=0;
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )=0;
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )=0;
	//
protected:
	Mob* m_pOwnedMob;
	Client* m_pOwnedClient;
	//
};

#endif InputModule_HPP