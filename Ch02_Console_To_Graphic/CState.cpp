#include "Include.h"
#include "CState.h"

CState::CState(char* stageData, int fileSize)
{
	SetSize(stageData, fileSize);
	m_Objects.SetSize(m_nWidth, m_nHeight);
	m_GoalFlags.SetSize(m_nWidth, m_nHeight);

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
void CState::DrawCell(int x, int y, unsigned color)
{
	unsigned* vram = GameLib::Framework::instance().videoMemory();
	unsigned windowWidth = GameLib::Framework::instance().width();

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			vram[(y * 16 + i) * windowWidth + (x * 16 + j)] = color;
		}
	}
}

void CState::Draw()
{
	for (int y = 0; y < m_nHeight; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			Object o = m_Objects(x, y);
			bool goalFlag = m_GoalFlags(x, y);
			unsigned color = 0;

			if (goalFlag)
			{
				switch (o)
				{
				case OBJ_SPACE:
					GameLib::cout << '.';
					color = 0x0000ff;
					break;
				case OBJ_WALL:
					GameLib::cout << '#';
					color = 0xffffff;
					break;
				case OBJ_BLOCK:
					GameLib::cout << 'O';
					color = 0xff00ff;
					break;
				case OBJ_PLAYER:
					GameLib::cout << 'P';
					color = 0x00ffff;
					break;
				}
			}
			else
			{
				switch (o)
				{
				case OBJ_SPACE:
					GameLib::cout << ' ';
					color = 0x000000;
					break;
				case OBJ_WALL:
					GameLib::cout << '#';
					color = 0xffffff;
					break;
				case OBJ_BLOCK:
					GameLib::cout << 'o';
					color = 0xff0000;
					break;
				case OBJ_PLAYER:
					GameLib::cout << 'p';
					color = 0x00ff00;
					break;
				}
			}
			DrawCell(x, y, color);
		}
		GameLib::cout << GameLib::endl;
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
	Array2D<Object>& o = m_Objects;

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
		if (o(tx2, tx2) == OBJ_SPACE)
		{
			o(tx2, ty2) = OBJ_BLOCK;
			o(tx, ty) = OBJ_PLAYER;
			o(x, y) = OBJ_SPACE;
		}
	}
}
