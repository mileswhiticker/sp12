#include "UID.hpp"

int NewUID()
{
	static int curUID = 0;
	return ++curUID;
}
