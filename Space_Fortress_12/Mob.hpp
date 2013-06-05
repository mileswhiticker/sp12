#ifndef MOB_HPP
#define MOB_HPP

#include <OIS\OISMouse.h>
#include <OIS\OISKeyboard.h>
#include <OGRE\OgreVector3.h>
#include <LinearMath\btQuaternion.h>

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
	enum MobType
	{
		UNKNOWN = 0,
		OBSERVER,
		HUMAN,
		ROBOT_DRONE,
		ALIEN_PENGUIN
		//
	};
	Mob(Ogre::Vector3 a_StartPos, int a_Direction = 0);
	~Mob();
	//
	virtual bool ConnectClient(Client* a_pNewClient);
	bool DisconnectClient();
	virtual void InstantiateAtom();
	virtual void SetupInputModules()=0;
	virtual void Update(float a_DeltaT);
	//
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	//
	MobType GetMobType();
	//used by input modules
	Client* m_pPossessingClient;
	//
	btQuaternion m_TargetStandingOrientation;
	float m_tLeftUprightOrientation;
	bool IsOnGround();
	Ogre::Vector3 GetCameraModelOffset();
	//
	bool UpdateOnGround();
	bool m_IsOnGround;
	//
protected:
	MobType m_MyMobType;
	std::vector<InputModule*> m_InputModules;
	Ogre::Vector3 m_CameraModelOffset;
	//
private:
	float m_tleftNextGroundRaycast;
	//
};

#endif	MOB_HPP