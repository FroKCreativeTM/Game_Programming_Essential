#pragma once

#include "Include.h"
#include "CImage.h"
#include "GameMode.h"

class CMenu
{
private : 
	CImage* m_menuImage;

public : 
	CMenu();
	~CMenu();

public : 
	void SelectMenu(bool& IsfirstGame,	GameMode& eGameMode, GameMode& eNextGameMode);
};

