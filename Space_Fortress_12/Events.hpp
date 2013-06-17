#ifndef EVENTS_HPP
#define EVENTS_HPP

namespace Event
{
	enum Intent
	{
		UNKNOWN_INTENT = 0,
		//
		HELP,
		GRAB,
		DISABLE,
		HARM,
	};

	enum Type
	{
		UNKNOWN_TYPE = 0,
		//
		TARGET,
		UNTARGET,
		//
		EX_ACT,
		EMP_ACT,
		//
		TOGGLEBUILD,
		BUILD,
		UNBUILD,
	};
};

#endif	EVENTS_HPP