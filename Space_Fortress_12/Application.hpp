#ifndef	APPLICATION_HPP
#define	APPLICATION_HPP

//#include <cegui\CEGUI.h>

#pragma warning(disable:4251)
#include <OGRE\OgreFrameListener.h>
#include <OGRE\OgreWindowEventUtilities.h>
#include <OIS\OISMouse.h>
#include <OIS\OISKeyboard.h>

/*#include <OIS\OISKeyboard.h>
#include <OIS\OISMouse.h>
#include <OGRE\SdkTrays.h>
#include <OGRE\OgreRenderQueueListener.h>*/

class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

class GameManager;
class Server;
class Client;
class InputHandler;
class Mob;
namespace Ogre
{
	class Root;
	class RenderWindow;
	class SceneManager;
	class Camera;
	struct FrameEvent;
};
namespace OIS
{
	class InputManager;
	class Keyboard;
	class Mouse;
};
namespace CEGUI
{
	class EventArgs;
	class Window;
};
class IHObserverCam;

class Application
:	public Ogre::FrameListener
,	public Ogre::WindowEventListener
,	public OIS::KeyListener
,	public OIS::MouseListener
/*,	OgreBites::SdkTrayListener
,	public Ogre::RenderQueueListener*/
{
public:
	static Application& GetSingleton();
	void Run();
	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual bool frameEnded(const Ogre::FrameEvent& evt);

	bool keyPressed( const OIS::KeyEvent &arg );
	bool keyReleased( const OIS::KeyEvent &arg );
	bool mouseMoved( const OIS::MouseEvent &arg );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool quit(const CEGUI::EventArgs &e);

	Ogre::SceneManager& GetSceneManager();
	static Ogre::SceneManager& StaticGetSceneManager();
	//
	btDiscreteDynamicsWorld& GetDynamicsWorld();
	static btDiscreteDynamicsWorld& StaticGetDynamicsWorld();
	//
	CEGUI::Window& GetCEGUIWindow();
	static CEGUI::Window& StaticGetCEGUIWindow();
	//
	Ogre::SceneNode& GetRootSceneNode();
	static Ogre::SceneNode& StaticGetRootSceneNode();
	//
	Ogre::RenderWindow& GetRenderWindow();
	static Ogre::RenderWindow& StaticGetRenderWindow();
	//
	void RegisterKeyListener(OIS::KeyListener* a_pKeyListener);
	static void StaticRegisterKeyListener(OIS::KeyListener* a_pKeyListener);
	//
	void RegisterMouseListener(OIS::MouseListener* a_pMouseListener);
	static void StaticRegisterMouseListener(OIS::MouseListener* a_pMouseListener);
	//
private:
	Application();

	//top level functions
	bool create();
	void createGui();
	void setupResources();
	bool Update(float a_DeltaT);

	//todo: move this to gamemanager
	void createScene();
	void createPhysics();
	void createTestClient();
	void createFrameListener();
	void createSphere(const std::string& strName, const float r, const int nRings = 16, const int nSegments = 16, bool a_inverted = false);
	
	Ogre::SceneNode* m_pRootSceneNode;
	
	//game manager
	GameManager& m_GameManager;

	//top level ogre
	Ogre::Root* m_pRoot;
    Ogre::RenderWindow* m_pWindow;
	Ogre::SceneManager* m_pSceneManager;
	bool m_ShutdownNextUpdate;

	//bullet physics
	btCollisionDispatcher* m_pBulletDispatcher;
	btBroadphaseInterface* m_pBulletOverlappingPairCache;
	btSequentialImpulseConstraintSolver* m_pBulletSolver;
	btDiscreteDynamicsWorld* m_pBulletDynamicsWorld;

	//UI
	CEGUI::Window* m_pCEGUIWindow;

	//OIS
	OIS::InputManager* m_pInputManager;
	OIS::Keyboard* m_pKeyboard;
	OIS::Mouse* m_pMouse;

	//networking, clients and mobs
	Server* m_pHostServer;
	std::vector<Client*> m_ConnectedClients;
	std::vector<Mob*> m_Mobs;
};

#endif	APPLICATION_HPP