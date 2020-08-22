#pragma once

#include "CArray2D.h"
#include "CObject.h"
#include "CImage.h"

// ���� ���� ������ �����Ѵ�.
class CState
{
// ��� ����
private : 
	int					m_nWidth;
	int					m_nHeight;
	CArray2D<CObject>	m_Objects;
	CImage*				m_Image;									// �̹���
	int					m_nMoveCount;								// �̵� �� ī��Ʈ
	static const int			M_MAX_MOVE_COUNT = 500;				// �̵� ���� �ʸ� ������ ���(500 * 1 = 0.5��) 

	// �Է� ���� bool ����
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
	void Update(int dt);		

	static int GetMaxMoveCount() { return M_MAX_MOVE_COUNT; }
};

