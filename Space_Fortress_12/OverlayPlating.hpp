#ifndef OVERLAY_PLATING_HPP
#define OVERLAY_PLATING_HPP

#include "Structure.hpp"
#include <OGRE\OgreVector3.h>

class OverlayPlating
:	public Structure
{
public:
	OverlayPlating(Ogre::Vector3 a_Pos);
	virtual void Instantiate(bool a_IsBuildPoint);
	//
private:
	//
};

#endif	OVERLAY_PLATING_HPP