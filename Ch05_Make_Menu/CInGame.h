#pragma once

#include "Include.h"
#include "CGameState.h"
#include "CImage.h"

class CInGame
{
private : 

public : 
	CInGame();
	~CInGame();

public : 
	void GameLoop(std::string strGameStageName);
};

