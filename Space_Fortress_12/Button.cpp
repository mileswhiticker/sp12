#include "Button.hpp"
#include "GlueScreen.hpp"

Button::Button(ButtonType a_Type)
:	m_MyType(a_Type)
{
	//
}

ButtonType Button::GetType()
{
	return m_MyType;
}
