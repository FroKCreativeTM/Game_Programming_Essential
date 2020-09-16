#include "Include.h"

/* ========= ��� �κ� ========= */
const char g_StageData[] = "\
########\
# .. P #\
# oo   #\
#      #\
########";
const int g_nStageWidth = 8;
const int g_nStageHeight = 5;

/* ========= �Լ� �κ� ========= */
void InitGameStage(Object* state, int w, int h, const char* stageData)
{
	const char* d = stageData;
	int x = 0;
	int y = 0;

	// NULL ���ڰ� �ƴҵ���
	while (*d != 0)
	{
		Object t;

		switch (*d)
		{
		case '#':	// ���� ���
			t = OBJ_WALL;
			break;
		case ' ':	// �� ������ ���
			t = OBJ_SPACE;
			break;
		case 'o':	// ����� ���
			t = OBJ_BLOCK;
			break;
		case 'O':	// ����� ������ ���� �ִ� ���
			t = OBJ_BLOCK_ON_GOAL;
			break;
		case '.':	// �������� ���
			t = OBJ_GOAL;
			break;
		case 'p':	// �÷��̾��� ���
			t = OBJ_PLAYER;
			break;
		case 'P':	// �÷��̾ ������ ���� �ִ� ���
			t = OBJ_PLAYER_ON_GOAL;
			break;
		case '\n':	// ������
			x = 0;	// �ϴ� x�� ���� ������ �ǵ�����.
			y++;	// y�� �������̴ϱ� �� �ܰ� �Ʒ���
			t = OBJ_UNKNOWN;	// ������ ����
			break;
		default:
			t = OBJ_UNKNOWN;	// ������ ����
			break;
		}
		++d;
		// ���� �����Ͱ� ���� ��� �����Ѵ�.
		if (t != OBJ_UNKNOWN)
		{
			state[y * w + x] = t;	// �� ������ �����͸� ����.
			x++;
		}
	}
}
bool checkClear(const Object* state, int w, int h)
{
	for (size_t i = 0; i < w * h; i++)
	{
		// ���� �������� �ƴ϶� �ʵ忡 �ִ� �ڽ�
		if (state[i] == OBJ_BLOCK)
		{
			return false;
		}
	}
	// ������ �ۿ� �ִ� �ڽ��� ���ٸ�!
	return true;
}
// ���� ������ �������� ������ ȭ���� �׸���.
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
// ������� �Է��� ������� �� ��ü���� �����Ѵ�.
void Update(Object* state, char input, int w, int h)
{
	// �̵����� ��ȯ�Ѵ�.
	int dx = 0;
	int dy = 0;

	switch (input)
	{
	case 'a' :	// �������� ���� ���
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
	default:
		break;
	}

	// �÷��̾� ��ǥ �˻�
	int i = -1;
	for (i = 0; i < w * h; i++)
	{
		// ã�Ҵٸ�!
		if (state[i] == OBJ_PLAYER || state[i] == OBJ_PLAYER_ON_GOAL)
		{
			break;
		}
	}
	// �÷��̾��� ��ǥ ���
	int x = i % w;
	int y = i / w;

	// �̵� �� ��ǥ
	int tx = x + dx;
	int ty = y + dy;

	// ��ǥ���� �ִ� �ּ� �˻�
	if (tx < 0 || ty < 0 || tx >= w || ty >= h)
	{
		return;
	}

	// A. �̵��� ���� ��ĭ �Ǵ� ������ -> �÷��̾� �̵�����
	int p = y * w + x;
	int tp = ty * w + tx;

	if (state[tp] == OBJ_SPACE || state[tp] == OBJ_GOAL)
	{
		// ���� ������
		// (���ǹ�) ? (true�� ���) : (false)�� ���

		// �̵��� ���� ��������� ������ �� ���, �ƴϸ� �׳� ���
		state[tp] = (state[tp] == OBJ_GOAL) ? OBJ_PLAYER_ON_GOAL : OBJ_PLAYER;

		// ���� �÷��̾��� ��ġ�� ������ ����� �������� �ƴϸ� ��������
		state[p] = (state[p] == OBJ_PLAYER_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
	}

	// B. ���� �̵��� ���� ���� -> �� ������ ����ĭ�� �����̰ų� ��������� �̵� ����
	else if (state[tp] == OBJ_BLOCK || state[tp] == OBJ_BLOCK_ON_GOAL)
	{
		// ���� ������ �˻��� ������
		int tx2 = tx + dx;
		int ty2 = ty + dy;

		// �� �� ����.
		// ������ â���� ������, ������ â ������ �� �о��.
		// �� �������� ������ �� �� ����!
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h)
		{
			return;
		}

		int tp2 = ty2 * w + tx2;	// 2ĭ ������

		// ���� ���� �ʸӰ� �����̳� �������� ���
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
	// ���� ���� �迭�� �Ҵ�޴´�.
	Object* state = new Object[g_nStageWidth * g_nStageHeight];
	// ���������� �ʱ�ȭ�Ѵ�.
	InitGameStage(state, g_nStageWidth, g_nStageHeight, g_StageData);

	// ���� ����
	while (true)
	{
		// �ϴ� ȭ�鿡 �׸���. (�ܼ��� �׸��� �� ���� ���̴�.)
		Draw(state, g_nStageWidth, g_nStageHeight);

		// Ŭ���� ������ Ȯ���Ѵ�.
		if (checkClear(state, g_nStageWidth, g_nStageHeight))
		{
			break; // ������ ���� ������.
		}
		// ������� �Է°��� �޴´�.
		// ���� Input �Լ��� ���� �д�.
		cout << "a:left, d:right, w:up, s:down. command : ";
		char input;
		cin >> input;

		// �Է¹��� ���� ������� ���������� �����Ѵ�.
		Update(state, input, g_nStageWidth, g_nStageHeight);
	}
	
	// ������ ������ �����Ѵ�.
	cout << "�����մϴ�. ����� �̰���ϴ�." << endl;

	// �Ҵ���� �޸𸮸� �����Ѵ�.
	delete[] state;
	state = nullptr;

	return 0;
}