#pragma once

#include "CArray2D.h"
#include "CObject.h"
#include "CImage.h"

// 게임 관련 전반을 관리한다.
class CState
{
// 멤버 변수
private : 
	int					m_nWidth;
	int					m_nHeight;
	CArray2D<CObject>	m_Objects;
	CImage*				m_Image;			// 이미지
	int					m_nMoveCount;		// 이동 중 카운트

	// 입력 관련 bool 변수
	bool m_PrevInputW = false;
	bool m_PrevInputA = false;
	bool m_PrevInputS = false;
	bool m_PrevInputD = false;

public : 
	CState(const char* stageData, int fileSize);
	~CState();

private : 
	void SetSize(const char* stageData, int size);

public : 
	bool checkClear();
	void Draw() const;
	void Update();
};

