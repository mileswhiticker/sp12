#ifndef MOB_HPP
#define MOB_HPP

#include <OIS\OISMouse.h>
#include <OIS\OISKeyboard.h>
#include <OGRE\OgreVector3.h>

#include "Atom.hpp"
class InputModule;
class ObserverFlight;
class Client;

class Mob
:	public OIS::KeyListener
,	public OIS::MouseListener
,	public Atom
{
public:
	Mob();
	//
	bool ConnectClient(Client* a_pNewClient);
	bool DisconnectClient();
	virtual void Update(float a_DeltaT);
	//
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	//
	//used by input modules
	Client* m_pPossessingClient;
	//
protected:
	std::vector<InputModule*> m_InputModules;
	//
};

#endif	MOB_HPP