#include "GlueScreen.hpp"

GlueScreen::GlueScreen()
:	m_QuitNextUpdate(false)
{
	//
}

bool GlueScreen::CheckQuitNextUpdate()
{
	return m_QuitNextUpdate;
}
