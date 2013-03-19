#ifndef BTOGRE_HELPER_HPP
#define BTOGRE_HELPER_HPP

#include <OGRE\OgreVector3.h>
#include <OGRE\OgreQuaternion.h>
#include <linearmath\btvector3.h>
#include <LinearMath\btQuaternion.h>

//code taken from OgreBullet wrapper
class OgreBtConverter
{
public:
    static btVector3 to(const Ogre::Vector3 &V)
    {
        return btVector3(V.x, V.y, V.z);
    };

    static btQuaternion to(const Ogre::Quaternion &Q)
    {
        return btQuaternion(Q.x, Q.y, Q.z, Q.w);
    };

};

class BtOgreConverter
{
public:
    static Ogre::Vector3 to(const btVector3 &V)
    {
        return Ogre::Vector3(V.x(), V.y(), V.z());
    };

    static Ogre::Quaternion to(const btQuaternion &Q)
    {
        return Ogre::Quaternion(Q.w(), Q.x(), Q.y(), Q.z());
    };
};

#define BT2OGRE BtOgreConverter::to
#define OGRE2BT OgreBtConverter::to

#endif	BTOGRE_HELPER_HPP