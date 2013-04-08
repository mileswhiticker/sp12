#include "Application.hpp"
#include "GlueScreen.hpp"

//#include "Input_handler.hpp"
#include "RandHelper.h"
#include "DebugDrawer.h"
#include "EffectManager.hpp"

#include <OGRE\OgreRoot.h>
#include <OGRE\OgreCamera.h>
#include <OGRE\OgreResourceGroupManager.h>
#include <OGRE\OgreConfigFile.h>
#include <OGRE\OgreSceneManager.h>
#include <OGRE\OgreRenderWindow.h>
#include <OGRE\OgreEntity.h>
#include <OGRE\OgreSubEntity.h>
#include <OGRE\OgreShadowCameraSetupFocused.h>

#include <OIS\OIS.h>

#include <CEGUI\CEGUI.h>
#include <cegui\RendererModules\Ogre\CEGUIOgreRenderer.h>

#include "Client.hpp"
#include "Mob.hpp"
#include "Observer.hpp"

#include "Files.hpp"
#include "MapSuite.hpp"

#include "Station.hpp"
#include "PlayerSpawn.hpp"

bool Application::create()
{
	m_pRoot = new Ogre::Root(PLUGINS_CFG);

	setupResources();
	
	//load configuration
	if(!m_pRoot->restoreConfig())
	{
		if(m_pRoot->showConfigDialog())
		{
			m_pWindow = m_pRoot->initialise(true, "Space Fortress 12");
		}
		else
			return false;
	}
	else
	{
		m_pWindow = m_pRoot->initialise(true, "Space Fortress 12");
	}

	//todo: setup window icon here
	//

	//m_pWindow->getCustomAttribute("");

	m_pSceneManager = m_pRoot->createSceneManager(Ogre::ST_GENERIC, "BaseSceneManager");
	//m_pSceneManager->setAmbientLight(Ogre::ColourValue(0, 0, 0));
	m_pRootSceneNode = m_pSceneManager->getRootSceneNode();

	createFrameListener();

	// Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// Load resources
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	//set up depth shadow mapping, now that we've loaded resources (requires custom shaders + materials)
	/*m_pSceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	// Allow self shadowing (note: this only works in conjunction with the shaders defined above)
	m_pSceneManager->setShadowTextureSelfShadow(true);
	// Set the caster material which uses the shaders defined above
	m_pSceneManager->setShadowTextureCasterMaterial("Ogre/DepthShadowmap/Caster/Float");
	// Set the pixel format to floating point
	m_pSceneManager->setShadowTexturePixelFormat(Ogre::PF_FLOAT32_R);
	// You can switch this on or off, I suggest you try both and see which works best for you
	m_pSceneManager->setShadowCasterRenderBackFaces(true);
	// Finally enable the shadows using texture additive integrated
	m_pSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);

	Ogre::FocusedShadowCameraSetup *camSetup = new Ogre::FocusedShadowCameraSetup();
	m_pSceneManager->setShadowCameraSetup(Ogre::ShadowCameraSetupPtr(camSetup));*/

	createPhysics();
	MapSuite::LoadMap("test");

	new DebugDrawer(m_pSceneManager, 0.1f);
	DebugDrawer::getSingleton().setEnabled(false);
	//no need for cleanup yet
	//delete DebugDrawer::getSingletonPtr();

	createGui();
	createTestClient();
	createScene();

	return true;
}

void Application::createFrameListener()
{
	//initialise OIS (input handling)
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
 
    m_pWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
 
    m_pInputManager = OIS::InputManager::createInputSystem( pl );
 
    m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputManager->createInputObject( OIS::OISKeyboard, true ));
    m_pMouse = static_cast<OIS::Mouse*>(m_pInputManager->createInputObject( OIS::OISMouse, true ));
	
    //Set initial mouse clipping size
    windowResized(m_pWindow);
 
    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(m_pWindow, this);
 
    m_pRoot->addFrameListener(this);
	m_pRoot->addFrameListener(EffectManager::GetSingletonPtr());
}

void Application::createScene()
{
	//create some boxes
	/*Ogre::Entity* pBoxEntity = m_pSceneManager->createEntity("boxEntity001", "full_tile_girder1Box002.mesh");
	int num = pBoxEntity->getNumSubEntities();
	for(int i=0;i<num;++i)
	{
		pBoxEntity->getSubEntity(i)->setMaterialName("girder_material");
	}
	Ogre::SceneNode* pBoxNode = m_pRootSceneNode->createChildSceneNode("boxNode001");
	pBoxNode->attachObject(pBoxEntity);
	pBoxNode->setPosition(1,0,0);
	
	pBoxEntity = m_pSceneManager->createEntity("boxEntity002", "full_tile_girder1Box002.mesh");
	num = pBoxEntity->getNumSubEntities();
	for(int i=0;i<num;++i)
	{
		pBoxEntity->getSubEntity(i)->setMaterialName("girder_material");
	}
	pBoxNode = m_pRootSceneNode->createChildSceneNode("boxNode002");
	pBoxNode->attachObject(pBoxEntity);
	pBoxNode->setPosition(1,0,1);

	pBoxEntity = m_pSceneManager->createEntity("boxEntity003", "full_tile_girder1Box002.mesh");
	num = pBoxEntity->getNumSubEntities();
	for(int i=0;i<num;++i)
	{
		pBoxEntity->getSubEntity(i)->setMaterialName("girder_material");
	}
	pBoxNode = m_pRootSceneNode->createChildSceneNode("boxNode003");
	pBoxNode->attachObject(pBoxEntity);
	pBoxNode->setPosition(1,0,-1);
	
	pBoxEntity = m_pSceneManager->createEntity("boxEntity004", "full_tile_girder1Box002.mesh");
	num = pBoxEntity->getNumSubEntities();
	for(int i=0;i<num;++i)
	{
		pBoxEntity->getSubEntity(i)->setMaterialName("girder_material");
	}
	pBoxNode = m_pRootSceneNode->createChildSceneNode("boxNode004");
	pBoxNode->attachObject(pBoxEntity);
	pBoxNode->setPosition(-1,0,0);

	pBoxEntity = m_pSceneManager->createEntity("boxEntity005", "full_tile_girder1Box002.mesh");
	num = pBoxEntity->getNumSubEntities();
	for(int i=0;i<num;++i)
	{
		pBoxEntity->getSubEntity(i)->setMaterialName("girder_material");
	}
	pBoxNode = m_pRootSceneNode->createChildSceneNode("boxNode005");
	pBoxNode->attachObject(pBoxEntity);
	pBoxNode->setPosition(-1,0,1);

	pBoxEntity = m_pSceneManager->createEntity("boxEntity006", "full_tile_girder1Box002.mesh");
	num = pBoxEntity->getNumSubEntities();
	for(int i=0;i<num;++i)
	{
		pBoxEntity->getSubEntity(i)->setMaterialName("girder_material");
	}
	pBoxNode = m_pRootSceneNode->createChildSceneNode("boxNode006");
	pBoxNode->attachObject(pBoxEntity);
	pBoxNode->setPosition(-1,0,-1);*/

	//create a sky dome
	createSphere("skydome", 1000000, 128, 128, true);
	Ogre::Entity* pSkydomeEntity = m_pSceneManager->createEntity("skydomeEntity", "skydome_mesh");
	pSkydomeEntity->getSubEntity(0)->setMaterialName("space_material");
	//
	m_pRootSceneNode->attachObject(pSkydomeEntity);
}

void Application::setupResources()
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(RESOURCES_CFG);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
 
    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
}

void Application::createGui()
{
	//bootstrap the renderer (let it handle initialisation etc)
	CEGUI::OgreRenderer& myRenderer = CEGUI::OgreRenderer::bootstrapSystem();

	//setup resource handling groups - cegui should automatically interface with ogre to allow ogre to handle all the resources
	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

	//create scheme manager - this is basically a single theme for the entire application
	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");

	//mouse cursor
	//CEGUI::System::getSingleton().setDefaultMouseCursor("WindowsLook", "MouseArrow");

	//root ui stuff
	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();

	m_pCEGUIWindow = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
	m_pCEGUIWindow->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
	CEGUI::System::getSingleton().setGUISheet(m_pCEGUIWindow);
}

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
//#include <BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h>

void Application::createPhysics()
{
	//collision configuration contains default setup for memory , collision setup . Advanced users can create their own configuration .
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	//use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras / BulletMultiThreaded)
	m_pBulletDispatcher = new btCollisionDispatcher(collisionConfiguration);

	//btDbvtBroadphase is a good general purpose broadphase . You can also try out btAxis3Sweep .
	m_pBulletOverlappingPairCache = new btDbvtBroadphase();

	//the default constraint solver. For parallel processing you can use a different solver (see Extras / BulletMultiThreaded)
	m_pBulletSolver = new btSequentialImpulseConstraintSolver;

	m_pBulletDynamicsWorld = new btDiscreteDynamicsWorld (m_pBulletDispatcher, m_pBulletOverlappingPairCache, m_pBulletSolver, collisionConfiguration);

	m_pBulletDynamicsWorld->setGravity(btVector3(0, -10, 0));

	//m_pBulletDynamicsWorld->
}

void Application::createTestClient()
{
	Client* pTestClient = new Client();
	m_ConnectedClients.push_back(pTestClient);

	//grab one of the loaded player spawns, and put the test client there
	//by default spawn at (10, 0, 0) and look west (down the negative x axis)
	Ogre::Vector3 spawnPos = Ogre::Vector3(10,5,0);
	int spawnDir = 8;
	PlayerSpawn* chosenSpawn = MapSuite::GetInstance().GetRandomPlayerSpawn();
	if(chosenSpawn)
	{
		spawnPos = chosenSpawn->pos;
		spawnDir = chosenSpawn->dir;
	}
	Observer* pTestObserver = new Observer(spawnPos, spawnDir);
	pTestObserver->ConnectClient(pTestClient);
	m_Mobs.push_back(pTestObserver);

	//grab a starting spawn
	Station* pStartingStation = MapSuite::GetStartingStation();
	if(pStartingStation)
	{
		//grab all observer spawn points
		std::vector<PlayerSpawn*> obsSpawns;
		for(auto it = pStartingStation->m_PlayerSpawns.begin(); it != pStartingStation->m_PlayerSpawns.end(); ++it)
		{
			if(!(*it)->type.compare("observer"))
				obsSpawns.push_back(*it);
		}

		int spawnMax = obsSpawns.size();
		if(spawnMax > 0)
		{
			int spawnIndex = iRand(0, spawnMax);
			PlayerSpawn* pObsSpawn = obsSpawns[spawnIndex];
			
			//grab the startpos
			pTestObserver->m_pAtomEntitySceneNode->setPosition(pObsSpawn->pos);

			//calculate the starting look direction
			Ogre::Vector3 startingLookDir = Ogre::Vector3(0,0,0);
			if(pObsSpawn->dir & 1)
				startingLookDir.z += 1;
			if(pObsSpawn->dir & 2)
				startingLookDir.z -= 1;
			if(pObsSpawn->dir & 4)
				startingLookDir.x += 1;
			if(pObsSpawn->dir & 8)
				startingLookDir.x -= 1;
			if(pObsSpawn->dir & 16)
				startingLookDir.y += 1;
			if(pObsSpawn->dir & 32)
				startingLookDir.y -= 1;
			pTestClient->m_pCamera->lookAt(pTestClient->m_pCamera->getDerivedPosition() + startingLookDir);
		}
		else
		{
			std::cout << "WARNING: No valid observer spawn landmarks!" << std::endl;
		}
	}
	else
	{
		//std::cout << "WARNING: Unable to load starting station!" << std::endl;
	}
}

#undef RESOURCES_CFG
#undef PLUGINS_CFG