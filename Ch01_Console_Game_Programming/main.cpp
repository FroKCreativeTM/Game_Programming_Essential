#include "Include.h"

/* ========= 상수 부분 ========= */
const char g_StageData[] = "\
########\
# .. P #\
# oo   #\
#      #\
########";
const int g_nStageWidth = 8;
const int g_nStageHeight = 5;

/* ========= 함수 부분 ========= */
void InitGameStage(Object* state, int w, int h, const char* stageData)
{
	const char* d = stageData;
	int x = 0;
	int y = 0;

	// NULL 문자가 아닐동안
	while (*d != 0)
	{
		Object t;

		switch (*d)
		{
		case '#':	// 벽인 경우
			t = OBJ_WALL;
			break;
		case ' ':	// 빈 공간의 경우
			t = OBJ_SPACE;
			break;
		case 'o':	// 블록의 경우
			t = OBJ_BLOCK;
			break;
		case 'O':	// 블록이 목적지 위에 있는 경우
			t = OBJ_BLOCK_ON_GOAL;
			break;
		case '.':	// 목적지인 경우
			t = OBJ_GOAL;
			break;
		case 'p':	// 플레이어의 경우
			t = OBJ_PLAYER;
			break;
		case 'P':	// 플레이어가 목적지 위에 있는 경우
			t = OBJ_PLAYER_ON_GOAL;
			break;
		case '\n':	// 다음행
			x = 0;	// 일단 x를 왼쪽 끝으로 되돌린다.
			y++;	// y는 다음행이니까 한 단계 아래로
			t = OBJ_UNKNOWN;	// 데이터 없음
			break;
		default:
			t = OBJ_UNKNOWN;	// 데이터 없음
			break;
		}
		++d;
		// 만약 데이터가 없는 경우 무시한다.
		if (t != OBJ_UNKNOWN)
		{
			state[y * w + x] = t;	// 그 공간의 데이터를 쓴다.
			x++;
		}
	}
}
bool checkClear(const Object* state, int w, int h)
{
	for (size_t i = 0; i < w * h; i++)
	{
		// 아직 목적지가 아니라 필드에 있는 박스
		if (state[i] == OBJ_BLOCK)
		{
			return false;
		}
	}
	// 목적지 밖에 있는 박스가 없다면!
	return true;
}
// 현재 정보를 바탕으로 게임의 화면을 그린다.
void Draw(const Object* state, int w, int h)
{
	const char cFont[] = { ' ','#', '.','o','O','p','P' };

	for (size_t y = 0; y < h; y++)
	{
		for (size_t x = 0; x < w; x++)
		{
			Object o = state[y * w + x];
			cout << cFont[o];
		}
		cout << endl;
	}
}
// 사용자의 입력을 받은대로 각 객체들을 갱신한다.
void Update(Object* state, char input, int w, int h)
{
	// 이동량을 반환한다.
	int dx = 0;
	int dy = 0;

	switch (input)
	{
	case 'a' :	// 왼쪽으로 가는 경우
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
	default:
		break;
	}

	// 플레이어 좌표 검색
	int i = -1;
	for (i = 0; i < w * h; i++)
	{
		// 찾았다면!
		if (state[i] == OBJ_PLAYER || state[i] == OBJ_PLAYER_ON_GOAL)
		{
			break;
		}
	}
	// 플레이어의 좌표 계산
	int x = i % w;
	int y = i / w;

	// 이동 후 좌표
	int tx = x + dx;
	int ty = y + dy;

	// 좌표에서 최대 최소 검사
	if (tx < 0 || ty < 0 || tx >= w || ty >= h)
	{
		return;
	}

	// A. 이동할 곳이 빈칸 또는 목적지 -> 플레이어 이동가능
	int p = y * w + x;
	int tp = ty * w + tx;

	if (state[tp] == OBJ_SPACE || state[tp] == OBJ_GOAL)
	{
		// 삼항 연산자
		// (조건문) ? (true인 경우) : (false)인 경우

		// 이동할 곳이 목적지라면 목적지 위 사람, 아니면 그냥 사람
		state[tp] = (state[tp] == OBJ_GOAL) ? OBJ_PLAYER_ON_GOAL : OBJ_PLAYER;

		// 현재 플레이어의 위치가 목적지 위라면 목적지로 아니면 공백으로
		state[p] = (state[p] == OBJ_PLAYER_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
	}

	// B. 만약 이동할 곳이 상자 -> 그 방향의 다음칸이 공백이거나 목적지라면 이동 가능
	else if (state[tp] == OBJ_BLOCK || state[tp] == OBJ_BLOCK_ON_GOAL)
	{
		// 상자 다음을 검사할 변수들
		int tx2 = tx + dx;
		int ty2 = ty + dy;

		// 밀 수 없다.
		// 윈도우 창으로 따지면, 윈도우 창 밖으로 못 밀어낸다.
		// 즉 스테이지 밖으로 밀 수 없다!
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h)
		{
			return;
		}

		int tp2 = ty2 * w + tx2;	// 2칸 앞으로

		// 만약 상자 너머가 공백이나 목적지인 경우
		if (state[tp2] == OBJ_SPACE || state[tp2] == OBJ_GOAL)
		{
			state[tp2] = (state[tp2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
			state[tp] = (state[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_PLAYER_ON_GOAL : OBJ_PLAYER;
			state[p] = (state[p] == OBJ_PLAYER_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
		}
	}
}

int main()
{
	// 게임 상태 배열을 할당받는다.
	Object* state = new Object[g_nStageWidth * g_nStageHeight];
	// 스테이지를 초기화한다.
	InitGameStage(state, g_nStageWidth, g_nStageHeight, g_StageData);

	// 게임 루프
	while (true)
	{
		// 일단 화면에 그린다. (콘솔은 그리는 게 제일 앞이다.)
		Draw(state, g_nStageWidth, g_nStageHeight);

		// 클리어 유무를 확인한다.
		if (checkClear(state, g_nStageWidth, g_nStageHeight))
		{
			break; // 루프를 빠져 나간다.
		}
		// 사용자의 입력값을 받는다.
		// 보통 Input 함수를 따로 둔다.
		cout << "a:left, d:right, w:up, s:down. command : ";
		char input;
		cin >> input;

		// 입력받은 값을 기반으로 스테이지를 갱신한다.
		Update(state, input, g_nStageWidth, g_nStageHeight);
	}
	
	// 게임이 끝나면 축하한다.
	cout << "축하합니다. 당신이 이겼습니다." << endl;

	// 할당받은 메모리를 해제한다.
	delete[] state;
	state = nullptr;

	return 0;
}