#ifndef FILES_HPP
#define FILES_HPP

#ifdef _DEBUG
	#define RESOURCES_CFG Ogre::String("resources_d.cfg")
	#define PLUGINS_CFG Ogre::String("plugins_d.cfg")
#else
	#define RESOURCES_CFG Ogre::String("resources.cfg")
	#define PLUGINS_CFG Ogre::String("plugins.cfg")
#endif

#define MEDIA_DIR std::string("../../media/")

#define MAP_DIR (MEDIA_DIR + std::string("maps/"))

#endif	FILES_HPP