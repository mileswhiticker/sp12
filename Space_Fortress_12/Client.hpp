#ifndef CLIENT_HPP
#define CLIENT_HPP

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
	Client();
	void Login();
	void Logout();
	void Update(float a_DeltaT);
	//
	void ResetCamera();
	//
	Ogre::Camera* m_pCamera;
	Ogre::SceneNode* m_pCameraNode;
	Mob* m_pPossessedMob;
	//
private:
	CEGUI::GUISheet* m_pTopInfoBar;
	bool m_HasPersonalCameraNode;
	//
};

#endif	CLIENT_HPP