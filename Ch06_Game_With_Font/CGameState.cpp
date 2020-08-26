#include "Include.h"
#include "CGameState.h"

CGameState::CGameState(const char* stageData, int fileSize)
{
	// 멤버 변수들 초기화
	SetSize(stageData, fileSize);
	m_Objects.SetSize(m_nWidth, m_nHeight);
	m_Image = new CImage("data/image/nimotsuKunImage2.dds");

	for (int y = 0; y < m_nHeight; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			m_Objects(x, y).m_eType = CObject::OBJ_WALL;	// 일단 벽으로 생성
		}
	}

	int x = 0;
	int y = 0;

	// 파일에 있는 모든 것을 쭉 돌아주며 데이터대로 저장한다.
	for (int i = 0; i < fileSize; i++)
	{
		CObject t;
		bool goalFlag = false;

		// 파일 데이터대로 다 저장한다.
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
	// 1. 배경을 그린다.
	for (int y = 0; y < m_nHeight; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			m_Objects(x, y).DrawBackGround(x, y, m_Image);
		}
	}
	// 2. 전경을 그린다.
	for (int y = 0; y < m_nHeight; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			m_Objects(x, y).DrawForeGround(x, y, m_Image, m_nMoveCount);
		}
	}
}

// @Param
// dt : 시간의 변화값을 전달하는 변수 
void CGameState::Update(int dt)
{
	// 이동 중 카운트가 M_MAX_MOVE_COUNT에 도달하면
	if (m_nMoveCount >= M_MAX_MOVE_COUNT)
	{
		// 초기화한다.
		m_nMoveCount = 0;

		// 이동 또한 초기화를 한다.
		for (int y = 0; y < m_nHeight; y++)
		{
			for (int x = 0; x < m_nWidth; x++)
			{
				m_Objects(x, y).m_nMoveX = 0;
				m_Objects(x, y).m_nMoveY = 0;
			}
		}
	}

	// 이동 중엔 갱신하지 않으므로 바로 return
	if (m_nMoveCount > 0)
	{
		m_nMoveCount += dt;		// 최댓값을 넘으면 안 되므로 최댓값으로 설정

		if (m_nMoveCount > M_MAX_MOVE_COUNT)
		{
			m_nMoveCount = M_MAX_MOVE_COUNT;
		}

		return;
	}

	// 이동량을 반환한다.
	int dx = 0;
	int dy = 0;

	// 프레임워크를 사용해서 isKeyOn을 사용한다.
	GameLib::Framework f = Framework::instance();

	// 새로운 입력 관련 변수
	bool inputW = f.isKeyOn('w');
	bool inputA = f.isKeyOn('a');
	bool inputS = f.isKeyOn('s');
	bool inputD = f.isKeyOn('d');

	if (inputW && (!m_PrevInputW))
	{	// 위쪽으로 가는 경우		
		dy = -1;
	} 
	else if (inputA && (!m_PrevInputA))
	{	// 왼쪽으로 가는 경우
		dx = -1;
	}
	else if (inputS && (!m_PrevInputS))
	{	// 오른쪽으로 가는 경우
		dy = 1;
	}
	else if (inputD && (!m_PrevInputD))
	{	// 오른쪽으로 가는 경우
		dx = 1;
	}
	else if (f.isKeyOn('q'))
	{
		return;
	}

	// 전 입력을 갱신한다.
	m_PrevInputW = inputW;
	m_PrevInputA = inputA;
	m_PrevInputS = inputS;
	m_PrevInputD = inputD;

	// 플레이어의 좌표 계산
	CArray2D<CObject>& o = m_Objects;

	int x = -1, y = -1;
	bool found = false;

	// 플레이어 좌표 검색
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

	// 이동 후 좌표
	int tx = x + dx;
	int ty = y + dy;

	// 좌표에서 최대 최소 검사
	if (tx < 0 || ty < 0 || tx >= m_nWidth || ty >= m_nHeight)
	{
		return;
	}

	// A. 이동할 곳이 빈칸 -> 플레이어 이동가능
	if (o(tx, ty).m_eType == CObject::OBJ_SPACE)
	{
		// 플레이어 이동
		o(tx, ty).Move(dx, dy, CObject::OBJ_PLAYER);
		// 플레이어가 있었던 자리는 공백
		o(x, y).Move(dx, dy, CObject::OBJ_SPACE);
		m_nMoveCount = 1;	// 이동 시작
	}
	// B. 만약 이동할 곳이 상자 -> 그 방향의 다음칸이 공백이면 이동 가능
	else if (o(tx, ty).m_eType == CObject::OBJ_BLOCK)
	{
		// 상자 다음을 검사할 변수들
		int tx2 = tx + dx;
		int ty2 = ty + dy;

		// 밀 수 없다.
		if (tx2 < 0 || ty2 < 0 || tx2 >= m_nWidth || ty2 >= m_nHeight)
		{
			return;
		}

		// 만약 상자 너머가 공백인 경우
		if (o(tx2, ty2).m_eType == CObject::OBJ_SPACE)
		{
			o(tx2, ty2).Move(dx, dy, CObject::OBJ_BLOCK);
			o(tx, ty).Move(dx, dy, CObject::OBJ_PLAYER);
			o(x, y).Move(dx, dy, CObject::OBJ_SPACE);
			m_nMoveCount = 1;	// 이동 시작
		}
	}
}