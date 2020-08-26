#include "Include.h"
#include "CGameState.h"

CGameState::CGameState(const char* stageData, int fileSize)
{
	// ��� ������ �ʱ�ȭ
	SetSize(stageData, fileSize);
	m_Objects.SetSize(m_nWidth, m_nHeight);
	m_Image = new CImage("data/image/nimotsuKunImage2.dds");

	for (int y = 0; y < m_nHeight; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			m_Objects(x, y).m_eType = CObject::OBJ_WALL;	// �ϴ� ������ ����
		}
	}

	int x = 0;
	int y = 0;

	// ���Ͽ� �ִ� ��� ���� �� �����ָ� �����ʹ�� �����Ѵ�.
	for (int i = 0; i < fileSize; i++)
	{
		CObject t;
		bool goalFlag = false;

		// ���� �����ʹ�� �� �����Ѵ�.
		switch (stageData[i])
		{
		case '#': 
		case ' ': 
		case 'o': 
		case 'O': 
		case '.': 
		case 'p': 
		case 'P': 
			m_Objects(x, y).Set(stageData[i]);
			x++;
			break;
		case '\n': 
			x = 0; 
			++y; 
			break; 
		}
	}
}

CGameState::~CGameState()
{
	if (m_Image)
	{
		delete m_Image;
	}
	m_Image = nullptr;
}

bool CGameState::checkClear()
{
	for (int y = 0; y < m_nHeight; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			if (m_Objects(x, y).m_eType == CObject::OBJ_BLOCK)
			{
				if (m_Objects(x, y).m_bGoalFlag == false)
				{
					return false;
				}
			}
		}
	}

	return true;
}

void CGameState::SetSize(const char* stageData, int size)
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

void CGameState::Draw() const
{
	// 1. ����� �׸���.
	for (int y = 0; y < m_nHeight; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			m_Objects(x, y).DrawBackGround(x, y, m_Image);
		}
	}
	// 2. ������ �׸���.
	for (int y = 0; y < m_nHeight; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			m_Objects(x, y).DrawForeGround(x, y, m_Image, m_nMoveCount);
		}
	}
}

// @Param
// dt : �ð��� ��ȭ���� �����ϴ� ���� 
void CGameState::Update(int dt)
{
	// �̵� �� ī��Ʈ�� M_MAX_MOVE_COUNT�� �����ϸ�
	if (m_nMoveCount >= M_MAX_MOVE_COUNT)
	{
		// �ʱ�ȭ�Ѵ�.
		m_nMoveCount = 0;

		// �̵� ���� �ʱ�ȭ�� �Ѵ�.
		for (int y = 0; y < m_nHeight; y++)
		{
			for (int x = 0; x < m_nWidth; x++)
			{
				m_Objects(x, y).m_nMoveX = 0;
				m_Objects(x, y).m_nMoveY = 0;
			}
		}
	}

	// �̵� �߿� �������� �����Ƿ� �ٷ� return
	if (m_nMoveCount > 0)
	{
		m_nMoveCount += dt;		// �ִ��� ������ �� �ǹǷ� �ִ����� ����

		if (m_nMoveCount > M_MAX_MOVE_COUNT)
		{
			m_nMoveCount = M_MAX_MOVE_COUNT;
		}

		return;
	}

	// �̵����� ��ȯ�Ѵ�.
	int dx = 0;
	int dy = 0;

	// �����ӿ�ũ�� ����ؼ� isKeyOn�� ����Ѵ�.
	GameLib::Framework f = Framework::instance();

	// ���ο� �Է� ���� ����
	bool inputW = f.isKeyOn('w');
	bool inputA = f.isKeyOn('a');
	bool inputS = f.isKeyOn('s');
	bool inputD = f.isKeyOn('d');

	if (inputW && (!m_PrevInputW))
	{	// �������� ���� ���		
		dy = -1;
	} 
	else if (inputA && (!m_PrevInputA))
	{	// �������� ���� ���
		dx = -1;
	}
	else if (inputS && (!m_PrevInputS))
	{	// ���������� ���� ���
		dy = 1;
	}
	else if (inputD && (!m_PrevInputD))
	{	// ���������� ���� ���
		dx = 1;
	}
	else if (f.isKeyOn('q'))
	{
		return;
	}

	// �� �Է��� �����Ѵ�.
	m_PrevInputW = inputW;
	m_PrevInputA = inputA;
	m_PrevInputS = inputS;
	m_PrevInputD = inputD;

	// �÷��̾��� ��ǥ ���
	CArray2D<CObject>& o = m_Objects;

	int x = -1, y = -1;
	bool found = false;

	// �÷��̾� ��ǥ �˻�
	for (y = 0; y < m_nHeight; ++y)
	{
		for (x = 0; x < m_nWidth; ++x)
		{
			if (o(x, y).m_eType == CObject::OBJ_PLAYER)
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
	if (o(tx, ty).m_eType == CObject::OBJ_SPACE)
	{
		// �÷��̾� �̵�
		o(tx, ty).Move(dx, dy, CObject::OBJ_PLAYER);
		// �÷��̾ �־��� �ڸ��� ����
		o(x, y).Move(dx, dy, CObject::OBJ_SPACE);
		m_nMoveCount = 1;	// �̵� ����
	}
	// B. ���� �̵��� ���� ���� -> �� ������ ����ĭ�� �����̸� �̵� ����
	else if (o(tx, ty).m_eType == CObject::OBJ_BLOCK)
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
		if (o(tx2, ty2).m_eType == CObject::OBJ_SPACE)
		{
			o(tx2, ty2).Move(dx, dy, CObject::OBJ_BLOCK);
			o(tx, ty).Move(dx, dy, CObject::OBJ_PLAYER);
			o(x, y).Move(dx, dy, CObject::OBJ_SPACE);
			m_nMoveCount = 1;	// �̵� ����
		}
	}
}