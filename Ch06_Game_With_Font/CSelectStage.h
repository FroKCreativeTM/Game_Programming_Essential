#pragma once

#include "Include.h"
#include "CImage.h"
#include "GameMode.h"

class CSelectStage
{
private : 
	CImage* m_selectStage;
	int m_nCursorPosition;

public :
	CSelectStage();
	~CSelectStage();

public : 
	void SelectStage(std::string& strGameStage, GameMode& eNextGameMode);
};

