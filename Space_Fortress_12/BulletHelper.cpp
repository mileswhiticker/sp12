#include "BulletHelper.hpp"
#include "Application.hpp"

btDiscreteDynamicsWorld& GetDynamicsWorld()
{
	return Application::StaticGetDynamicsWorld();
}
