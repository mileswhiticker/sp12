#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
class Mob;
namespace CEGUI
{
	class GUISheet;
};

namespace Ogre
{
	class Camera;
	class SceneNode;
};

//placeholder for networked client
class Client
{
public:
	enum MessageType
	{
		MSG_UNKNOWN = 0,
		//
		FRIENDLY,
		NEUTRAL,
		WARNING,
		HOSTILE,
		//
		MSG_MAX
	};
	Client();
	void Login();
	void Logout();
	void DisplayMessage(std::string a_Message, MessageType a_MsgType);

	void Update(float a_DeltaT);
	//
	void ResetCamera();
	//
	Ogre::Camera* m_pCamera;
	Ogre::SceneNode* m_pCameraNode;
	Mob* m_pPossessedMob;
	//
	CEGUI::GUISheet* m_pTopInfoBar;
	CEGUI::GUISheet* m_pFPSCounter;
private:
	bool m_HasPersonalCameraNode;
	//
};

#endif	CLIENT_HPP