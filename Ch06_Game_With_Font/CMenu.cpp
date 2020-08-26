#include "CMenu.h"

CMenu::CMenu() : 
	m_menuImage(nullptr)
{
	m_menuImage = new CImage("data/image/menu.dds");
}

CMenu::~CMenu()
{
	SAFE_DELETE(m_menuImage);
}

void CMenu::SelectMenu(bool& IsfirstGame,
	GameMode& eGameMode,
	GameMode& eNextGameMode)
{
	GameLib::Framework f = GameLib::Framework::instance();

	m_menuImage->Draw(m_menuImage->GetWidth(), m_menuImage->GetHeight());

	if (f.isKeyOn('a') && !IsfirstGame)
	{
		eGameMode = GameMode::GM_INGAME;
	}
	else if (f.isKeyOn('s'))
	{
		eGameMode = GameMode::GM_SELECTSTAGE;
		eNextGameMode = GameMode::GM_SELECTSTAGE;
		IsfirstGame = false;
	}
	else if (f.isKeyOn('d'))
	{
		eGameMode = GameMode::GM_TITLE;
	}
	else if (f.isKeyOn('f'))
	{
		GameLib::Framework::instance().requestEnd();
	}
}
