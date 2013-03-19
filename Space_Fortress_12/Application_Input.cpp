#include "Application.hpp"

#include <OGRE\OgreCamera.h>
#include <OGRE\OgreSceneNode.h>
#include <OGRE\OgreSceneManager.h>

#include <cegui\CEGUISystem.h>

//todo: move this to helper file?
CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;
 
    case OIS::MB_Right:
        return CEGUI::RightButton;
 
    case OIS::MB_Middle:
        return CEGUI::MiddleButton;
 
    default:
        return CEGUI::LeftButton;
    }
}

bool Application::keyPressed( const OIS::KeyEvent &arg )
{
	if(arg.key == OIS::KC_ESCAPE)
		exit(0);
	//
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectKeyDown(arg.key);
	sys.injectChar(arg.text);
	return true;
}

bool Application::keyReleased( const OIS::KeyEvent &arg )
{
	CEGUI::System::getSingleton().injectKeyUp(arg.key);
	return true;
}

bool Application::mouseMoved( const OIS::MouseEvent &arg )
{
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectMouseMove( float(arg.state.X.rel), float(arg.state.Y.rel));
	// Scroll wheel.
	if (arg.state.Z.rel)
		sys.injectMouseWheelChange(arg.state.Z.rel / 120.0f);
	return false;
}

bool Application::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));
	return true;
}

bool Application::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));
	return true;
}

bool Application::quit(const CEGUI::EventArgs &e)
{
    return true;
}
