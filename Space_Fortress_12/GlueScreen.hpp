#ifndef	GLUE_SCREEN_HPP
#define	GLUE_SCREEN_HPP

#include "Button.hpp"
#include <Gwen\Gwen.h>

namespace CEGUI
{
	class Window;
};

class GlueScreen
{
public:
	GlueScreen();
	virtual void Update(float a_DeltaT)=0;
	//
	virtual void Create()=0;
	virtual void Destroy()=0;
	//
	bool CheckQuitNextUpdate();
	//virtual HandleEvent
protected:
	bool m_QuitNextUpdate;
	//
};

class MainMenu
:	public GlueScreen
{
public:
	MainMenu(bool a_BootstrapImmediately = true);
	virtual void Update(float a_DeltaT);
	//
	virtual void Create();
	virtual void Destroy();
	//
private:
	CEGUI::Window* m_pMenuTitle;
	CEGUI::Window* m_pAuthorTitle;
	CEGUI::Window* m_pMenuBackground;
	CEGUI::Window* m_pNewGameButton;
	CEGUI::Window* m_pQuitButton;
	//
};

#endif	GLUE_SCREEN_HPP