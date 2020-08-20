#include "Include.h"
#include "CState.h"

CState::CState(const char* stageData, int fileSize)
{
	// ��� ������ �ʱ�ȭ
	SetSize(stageData, fileSize);
	m_Objects.SetSize(m_nWidth, m_nHeight);
	m_GoalFlags.SetSize(m_nWidth, m_nHeight);
	m_Image = new CImage("nimotsuKunImage.dds");

	for (int y = 0; y < m_nHeight; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			m_Objects(x, y) = OBJ_WALL;	// �ϴ� ������ ����
			m_GoalFlags(x, y) = false;	// �ϴ� ������ �÷��׵� �� ���д�.
		}
	}

	int x = 0;
	int y = 0;

	// ���Ͽ� �ִ� ��� ���� �� �����ָ� �����ʹ�� �����Ѵ�.
	for (int i = 0; i < fileSize; i++)
	{
		Object t;
		bool goalFlag = false;

		// ���� �����ʹ�� �� �����Ѵ�.
		switch (stageData[i])
		{
		case '#': 
			t = OBJ_WALL; 
			break;
		case ' ': 
			t = OBJ_SPACE; 
			break;
		case 'o': 
			t = OBJ_BLOCK; 
			break;
		case 'O': 
			t = OBJ_BLOCK; 
			goalFlag = true; 
			break;
		case '.': 
			t = OBJ_SPACE; 
			goalFlag = true; 
			break;
		case 'p': 
			t = OBJ_PLAYER; 
			break;
		case 'P': 
			t = OBJ_PLAYER; 
			goalFlag = true; 
			break;
		case '\n': 
			x = 0; 
			++y; 
			t = OBJ_UNKNOWN; 
			break; 
		default: 
			t = OBJ_UNKNOWN; 
			break;
		}

		if (t != OBJ_UNKNOWN) 
		{ 
			m_Objects(x, y) = t; // ���� ���º��� ��Ī�Ѵ�.
			m_GoalFlags(x, y) = goalFlag; // �÷��׸� ��Ī�Ѵ�.
			++x;
		}
	}
}

bool CState::checkClear()
{
	for (int y = 0; y < m_nHeight; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			if (m_Objects(x, y) == OBJ_BLOCK)
			{
				if (m_GoalFlags(x, y) == false) 
				{
					return false;
				}
			}
		}
	}

	return true;
}

void CState::SetSize(const char* stageData, int size)
{
	m_nWidth = m_nHeight = 0;

	int x = 0;
	int y = 0;
	for (int i = 0; i < size; ++i) 
	{
		switch (stageData[i]) 
		{
		case '#': case ' ': case 'o': case 'O':
		case '.': case 'p': case 'P':
			++x;
			break;
		case '\n':
			++y;
			m_nWidth = std::max(m_nWidth, x);
			m_nHeight = std::max(m_nHeight, y);
			x = 0;
			break;
		}
	}
}

// �ȼ��� �׸����� ��ȯ�ϱ� ���� �޼���
void CState::DrawCell(int x, int y, ImageID id) const
{
	m_Image->Draw(x * 32, y * 32, id * 32, 0, 32, 32);
}

void CState::Draw()
{
	for (int y = 0; y < m_nHeight; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			Object o = m_Objects(x, y);
			bool goalFlag = m_GoalFlags(x, y);
			
			if (o != OBJ_WALL)
			{
				if (goalFlag)
				{
					DrawCell(x, y, II_GOAL);
				}
				else
				{
					DrawCell(x, y, II_SPACE);
				}
			}

			ImageID id = II_SPACE;
			switch (o)
			{
			case OBJ_WALL: id = II_WALL; break;
			case OBJ_BLOCK: id = II_BLOCK; break;
			case OBJ_PLAYER: id = II_PLAYER; break;
			}

			if (id != II_SPACE)
			{
				DrawCell(x, y, id);
			}
		}
	}
}

void CState::Update(char input)
{
	// �̵����� ��ȯ�Ѵ�.
	int dx = 0;
	int dy = 0;

	switch (input)
	{
	case 'a':	// �������� ���� ���
		dx = -1;
		break;
	case 'd':	// ���������� ���� ���
		dx = 1;
		break;
	case 'w':	// �������� ���� ���
		dy = -1;
		break;
	case 's':	// ���������� ���� ���
		dy = 1;
		break;
	}

	// �÷��̾��� ��ǥ ���
	CArray2D<Object>& o = m_Objects;

	int x = -1, y = -1;
	bool found = false;

	// �÷��̾� ��ǥ �˻�
	for (y = 0; y < m_nHeight; ++y)
	{
		for (x = 0; x < m_nWidth; ++x)
		{
			if (o(x, y) == OBJ_PLAYER)
			{
				found = true;
				break;
			}
		}
		if (found)
		{
			break;
		}
	}

	// �̵� �� ��ǥ
	int tx = x + dx;
	int ty = y + dy;

	// ��ǥ���� �ִ� �ּ� �˻�
	if (tx < 0 || ty < 0 || tx >= m_nWidth || ty >= m_nHeight)
	{
		return;
	}

	// A. �̵��� ���� ��ĭ -> �÷��̾� �̵�����
	if (o(tx, ty) == OBJ_SPACE)
	{
		// �÷��̾� �̵�
		o(tx, ty) = OBJ_PLAYER;
		// �÷��̾ �־��� �ڸ��� ����
		o(x, y) = OBJ_SPACE;
	}
	// B. ���� �̵��� ���� ���� -> �� ������ ����ĭ�� �����̸� �̵� ����
	else if (o(tx, ty) == OBJ_BLOCK)
	{
		// ���� ������ �˻��� ������
		int tx2 = tx + dx;
		int ty2 = ty + dy;

		// �� �� ����.
		if (tx2 < 0 || ty2 < 0 || tx2 >= m_nWidth || ty2 >= m_nHeight)
		{
			return;
		}

		// ���� ���� �ʸӰ� ������ ���
		if (o(tx2, ty2) == OBJ_SPACE)
		{
			o(tx2, ty2) = OBJ_BLOCK;
			o(tx, ty) = OBJ_PLAYER;
			o(x, y) = OBJ_SPACE;
		}
	}
}
