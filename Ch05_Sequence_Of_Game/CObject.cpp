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
	// �����̴� ���� ����� ���� ���̴�.
	ImageID id = II_SPACE;				// ������ ���ٴ� �ǹ̷� ����Ѵ�.

	if (m_eType == OBJ_BLOCK)
	{	// ����� ��� �̹��� ���̵�� ���
		id = II_BLOCK;
	}
	else if (m_eType == OBJ_PLAYER)
	{	// ����� ��� �̹��� ���̵�� ���
		id = II_PLAYER;
	}

	if (id != II_SPACE)	// ����� �ƴ϶�� �̵� ����!
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
