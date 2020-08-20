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
			m_Objects(x, y) = OBJ_WALL;	// 일단 벽으로 생성
			m_GoalFlags(x, y) = false;	// 일단 목적지 플래그도 다 꺼둔다.
		}
	}

	int x = 0;
	int y = 0;

	// 파일에 있는 모든 것을 쭉 돌아주며 데이터대로 저장한다.
	for (int i = 0; i < fileSize; i++)
	{
		Object t;
		bool goalFlag = false;

		// 파일 데이터대로 다 저장한다.
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
			m_Objects(x, y) = t; // 맵을 상태별로 매칭한다.
			m_GoalFlags(x, y) = goalFlag; // 플래그를 매칭한다.
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

// 픽셀을 그림으로 변환하기 위한 메서드
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
	// 이동량을 반환한다.
	int dx = 0;
	int dy = 0;

	switch (input)
	{
	case 'a':	// 왼쪽으로 가는 경우
		dx = -1;
		break;
	case 'd':	// 오른쪽으로 가는 경우
		dx = 1;
		break;
	case 'w':	// 위쪽으로 가는 경우
		dy = -1;
		break;
	case 's':	// 오른쪽으로 가는 경우
		dy = 1;
		break;
	}

	// 플레이어의 좌표 계산
	Array2D<Object>& o = m_Objects;

	int x = -1, y = -1;
	bool found = false;

	// 플레이어 좌표 검색
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

	// 이동 후 좌표
	int tx = x + dx;
	int ty = y + dy;

	// 좌표에서 최대 최소 검사
	if (tx < 0 || ty < 0 || tx >= m_nWidth || ty >= m_nHeight)
	{
		return;
	}

	// A. 이동할 곳이 빈칸 -> 플레이어 이동가능
	if (o(tx, ty) == OBJ_SPACE)
	{
		// 플레이어 이동
		o(tx, ty) = OBJ_PLAYER;
		// 플레이어가 있었던 자리는 공백
		o(x, y) = OBJ_SPACE;
	}
	// B. 만약 이동할 곳이 상자 -> 그 방향의 다음칸이 공백이면 이동 가능
	else if (o(tx, ty) == OBJ_BLOCK)
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
		if (o(tx2, tx2) == OBJ_SPACE)
		{
			o(tx2, ty2) = OBJ_BLOCK;
			o(tx, ty) = OBJ_PLAYER;
			o(x, y) = OBJ_SPACE;
		}
	}
}
