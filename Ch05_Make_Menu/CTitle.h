#pragma once

#include "Include.h"
#include "CImage.h"
#include "GameMode.h"

class CTitle
{
private : 
	CImage* m_titleImage;

public : 
	CTitle();
	~CTitle();

public : 
	void Draw(GameMode& eGameMode);
};

