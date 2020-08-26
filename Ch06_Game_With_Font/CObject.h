#pragma once

#include "CImage.h"

class CObject
{
private:
	enum Type
	{
		OBJ_SPACE,
		OBJ_WALL,
		OBJ_BLOCK,
		OBJ_PLAYER,

		OBJ_UNKNOWN
	};

	enum ImageID
	{
		II_PLAYER,
		II_WALL,
		II_BLOCK,
		II_GOAL,
		II_SPACE
	};

	Type	m_eType;
	bool	m_bGoalFlag;
	int		m_nMoveX;
	int		m_nMoveY;

	friend class CGameState;		// CState���� �ٷ� ���� �����ϵ���

public:
	CObject();
	~CObject();

public:
	// �������� �������� ���ڷ� �ڽ��� �ʱ�ȭ
	void Set(char c);
	// �׸���, ���, ���� ��
	void DrawBackGround(int x, int y, const CImage* image) const;
	void DrawForeGround(int x, int y, const CImage* image, int moveCount) const;
	// ������Ʈ�� �̵�
	void Move(int dx, int dy, Type eReplaceType);
	// ���� �׸��� �޼���
	static void DrawCell(int x, int y, int id, const CImage* image);
};