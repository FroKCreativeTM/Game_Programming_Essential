#include "CObject.h"

CObject::CObject() : 
	m_eType(OBJ_WALL),
	m_bGoalFlag(false),
	m_nMoveX(0),
	m_nMoveY(0)
{
}

CObject::~CObject()
{
}

void CObject::Set(char c)
{
	switch (c) 
	{
	case '#': 
		m_eType = OBJ_WALL; 
		break;
	case ' ': 
		m_eType = OBJ_SPACE; 
		break;
	case 'o': 
		m_eType = OBJ_BLOCK; 
		break;
	case 'O': 
		m_eType = OBJ_BLOCK; 
		m_bGoalFlag = true; 
		break;
	case '.': 
		m_eType = OBJ_SPACE;
		m_bGoalFlag = true; 
		break;
	case 'p': 
		m_eType = OBJ_PLAYER; 
		break;
	case 'P': 
		m_eType = CObject::OBJ_PLAYER; 
		m_bGoalFlag = true;
		break;
	}
}

void CObject::DrawBackGround(int x, int y, const CImage* image) const
{
	ImageID id = II_SPACE;

	if (m_eType == OBJ_WALL)
	{
		DrawCell(x, y, II_WALL, image);
	}
	else
	{
		if (m_bGoalFlag)
		{
			DrawCell(x, y, II_GOAL, image);
		}
		else
		{
			DrawCell(x, y, II_SPACE, image);
		}
	}
}

void CObject::DrawForeGround(int x, int y, const CImage* image, int moveCount) const
{
	// 움직이는 것은 사람과 상자 뿐이다.
	ImageID id = II_SPACE;				// 전경이 없다는 의미로 사용한다.

	if (m_eType == OBJ_BLOCK)
	{	// 블록의 경우 이미지 아이디는 블록
		id = II_BLOCK;
	}
	else if (m_eType == OBJ_PLAYER)
	{	// 블록의 경우 이미지 아이디는 블록
		id = II_PLAYER;
	}

	if (id != II_SPACE)	// 배경이 아니라면 이동 시작!
	{
		int dx = m_nMoveX * (32 - moveCount);
		int dy = m_nMoveY * (32 - moveCount);
		image->Draw(x * 32 - dx, y * 32 - dy, id * 32, 0, 32, 32);
	}
}

void CObject::Move(int dx, int dy, Type eReplaceType)
{
	m_nMoveX = dx;
	m_nMoveY = dy;
	m_eType = eReplaceType;
}

void CObject::DrawCell(int x, int y, int id, const CImage* image)
{
	image->Draw(x * 32, y * 32, id * 32, 0, 32, 32);
}
