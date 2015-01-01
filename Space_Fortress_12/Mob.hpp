#ifndef MOB_HPP
#define MOB_HPP

#include <OIS\OISMouse.h>
#include <OIS\OISKeyboard.h>
#include <OGRE\OgreVector3.h>
#include <LinearMath\btQuaternion.h>
#include <map>
#include <vector>

#include "Atom.hpp"

class InputModule;
class ObserverFlight;
class Client;
class Object;

struct RaycastingInputModule
{
	InputModule* pInputModule;
	int TargetFlags;
};

class Mob
:	public OIS::KeyListener
,	public OIS::MouseListener
,	public Atom
{
public:
	friend class InputModule;
	enum MobType
	{
		UNKNOWN = 0,
		OBSERVER,
		HUMANOID,
		ROBOT_DRONE,
		ALIEN_PENGUIN
		//
	};
	enum IntentType
	{
		INTENT_UNKNOWN = 0,
		//
		HELP,
		DISARM,
		GRAB,
		HARM,
		//
		INTENT_MAX
	};
	enum InteractType
	{
		INTERACT_UNKNOWN = 0,
		//
		TARGET,
		UNTARGET,
		INTERACT,
		INTERACT_END,
		//
		INTERACT_MAX
	};
	Mob(Ogre::Vector3 a_StartPos, int a_Direction = 0);
	~Mob();
	//
	virtual bool ConnectClient(Client* a_pNewClient);
	bool DisconnectClient();
	virtual void InstantiateAtom();
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
	bool AddInputModule(InputModule* a_pNewInputModule, bool a_Active = true);
	bool DeactivateInputModule(InputModule* a_pInputModuleToDeactivate);
	bool ReactivateInputModule(InputModule* a_pInputModuleToReactivate);
	//
	bool ClearInputModule(InputModule* a_pInputModuleToClear, bool a_Active = true);
	bool ClearActiveInputModule(InputModule* a_pInputModuleToClear);
	bool ClearInactiveInputModule(InputModule* a_pInputModuleToClear);

	btQuaternion m_TargetStandingOrientation;
	float m_tLeftUprightOrientation;
	bool IsOnGround();
	Ogre::Vector3 GetCameraModelOffset();
	//
	bool UpdateOnGround();
	bool m_IsOnGround;
	IntentType GetIntent();
	virtual bool OnGravityChange();
	virtual void UpdateOrientation(float a_DeltaT);
	//
	void SendClientMessage(std::string a_Message, int a_MsgType = 2);
	//
protected:
	MobType m_MyMobType;
	Ogre::Vector3 m_CameraModelOffset;
	IntentType m_Intent;
	Object* m_pHeldObject;
	//
	std::vector<InputModule*> m_ActiveInputModules;
	std::vector<InputModule*> m_InactiveInputModules;
	//
	InputModule* m_pCurrentRaycastingInputModule;
	void RaycastForward();
	//
	float m_tLeftInteractRaycast;
	float m_InteractRaycastInterval;
	//
private:
	float m_tleftNextGroundRaycast;
	//
};

#endif	MOB_HPP