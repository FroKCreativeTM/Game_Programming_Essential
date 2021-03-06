#pragma once

#include "CArray2D.h"
#include "CObject.h"
#include "CImage.h"

// 게임 관련 전반을 관리한다.
class CGameState
{
// 멤버 변수
private : 
	int					m_nWidth;
	int					m_nHeight;
	CArray2D<CObject>	m_Objects;
	CImage*				m_Image;									// 이미지
	int					m_nMoveCount;								// 이동 중 카운트
	static const int			M_MAX_MOVE_COUNT = 500;				// 이동 종료 초를 결정할 상수(500 * 1 = 0.5초) 

	// 입력 관련 bool 변수
	bool m_PrevInputW = false;
	bool m_PrevInputA = false;
	bool m_PrevInputS = false;
	bool m_PrevInputD = false;

public : 
	CGameState(const char* stageData, int fileSize);
	~CGameState();

private : 
	void SetSize(const char* stageData, int size);

public : 
	bool checkClear();
	void Draw() const;
	void Update(int dt);		

	static int GetMaxMoveCount() { return M_MAX_MOVE_COUNT; }
};

