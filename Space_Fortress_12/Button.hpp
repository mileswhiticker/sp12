#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <string>

class GlueScreen;

enum ButtonType
{
	BUTTON_INVALID = 0,
	//
	QUIT_BUTTON,
	NEWGAME_BUTTON,
	//
	BUTTON_MAX
};

class Button
{
public:
	//
	Button(ButtonType a_Type);
	ButtonType GetType();
	//
private:
	ButtonType m_MyType;
	//
};

#endif	BUTTON_HPP