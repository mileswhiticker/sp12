#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <time.h>

#include "guicon.h"
#include "Application.hpp"

#pragma warning(disable:4251)
#include <OGRE\OgreException.h>

int __stdcall WinMain ( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd_line, int cmd_show )
{
	RedirectIOToConsole();
	
	srand((unsigned int)time(0));

	//new instance of the application
	try 
	{
		Application::GetSingleton().Run();
	}
	catch( Ogre::Exception& e )
	{
		std::cout << e.getFullDescription().c_str() << std::endl;
		MessageBox( NULL, L"Ogre Exception", L"An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
    }

    return 0;
}
