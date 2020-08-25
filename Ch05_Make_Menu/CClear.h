#pragma once

#include "Include.h"
#include "CImage.h"

class CClear
{
private : 
	CImage* m_clearImage;

public : 
	CClear();
	~CClear();

public : 
	void Draw();
};

