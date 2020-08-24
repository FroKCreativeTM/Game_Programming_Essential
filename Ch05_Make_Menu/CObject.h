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

	friend class CGameState;		// CState에선 바로 접근 가능하도록

public:
	CObject();
	~CObject();

public:
	// 스테이지 데이터의 문자로 자신을 초기화
	void Set(char c);
	// 그리기, 배경, 전경 순
	void DrawBackGround(int x, int y, const CImage* image) const;
	void DrawForeGround(int x, int y, const CImage* image, int moveCount) const;
	// 오브젝트의 이동
	void Move(int dx, int dy, Type eReplaceType);
	// 셀을 그리는 메서드
	static void DrawCell(int x, int y, int id, const CImage* image);
};