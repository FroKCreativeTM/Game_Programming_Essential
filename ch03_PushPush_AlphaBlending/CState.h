#pragma once

#include "CArray2D.h"
#include "Object.h"
#include "CImage.h"

// 게임 관련 전반을 관리한다.
class CState
{
private : 
	int					m_nWidth;
	int					m_nHeight;
	CArray2D<Object>	m_Objects;
	CArray2D<bool>		m_GoalFlags;
	CImage*				m_Image;

public : 
	CState(const char* stageData, int fileSize);
	~CState();

private : 
	void SetSize(const char* stageData, int size);
	void DrawCell(int x, int y, ImageID id) const;

public : 
	bool checkClear();
	void Draw();
	void Update(char input);
};

