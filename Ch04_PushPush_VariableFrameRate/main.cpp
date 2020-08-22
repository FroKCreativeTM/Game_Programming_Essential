#include "Include.h"
#include "CState.h"
#include "CFileSystem.h"

/* ========== ���� ���� ========== */
CState* g_GameState = nullptr;		// ������ ���¸� �����ϱ� ���� Ŭ�����Դϴ�.
const int gFrameInterval = 16;		// 16�и�������(60������)
unsigned gPreviousTime[10];			// �� �ð��� �����صα� ���� �迭
int gCounter = 0;					// �������� �󸶳� ����Ǿ����� �����Ѵ�.


/* ========== ���� �Լ� ========== */
void MainLoop()
{
	Framework f = Framework::instance();

	f.sleep(1);	// 1 �и��ʾ��� ����� CPU�� �� ������ ��� �����ϴ� �� ����..

	// ���� �������� �����ϱ� ���� ������ �����Ѵ�.
	unsigned currentTime = f.time();
	unsigned frameInterval = currentTime - gPreviousTime[9];
	unsigned frameInterval10 = currentTime - gPreviousTime[0];

	if (gCounter % 60 == 0)
	{
		GameLib::cout << "FrameInterval : " << frameInterval10 / 10;
		GameLib::cout << " FrameRate : " << 10 * 1000 / frameInterval10 << GameLib::endl;
	}
	gCounter++;

	for (int i = 0; i < 9; i++)
	{
		gPreviousTime[i] = gPreviousTime[i + 1];
	}
	gPreviousTime[9] = currentTime;

	// ���� ���� ���� Ŭ������ �������� ���� ���
	// �� ������ ���� ����̴�.
	if (!g_GameState)
	{
		// ���������� �����´�.
		const char* strStageName = "Stage1.txt";

		// ���� �ý����� �ҷ��´�.
		CFileSystem fileSystem(strStageName);

		// ���� ���������� ������ �ʾҴٸ�
		if (!fileSystem.GetData())
		{
			GameLib::cout << "Stage file could not be read" << GameLib::endl;
			return;
		}

		// ���� ���� Ŭ������ �����Ѵ�.
		g_GameState = new CState(fileSystem.GetData(), fileSystem.GetFileSize());

		// �׸���.
		g_GameState->Draw();
		return; // �Լ��� �����Ѵ�.
	}

	// Ŭ���� ���θ� �˻��ϱ� ���� Ŭ���� ���� ������ �����Ѵ�.
	bool isCleared = false;

	if (g_GameState->checkClear())
	{
		isCleared = true;
	}


	// �Է��� �������� ������Ʈ �Ѵ�.
	g_GameState->Update(frameInterval);
	// �̸� �������� �׸���.
	g_GameState->Draw();

	// ���� Ŭ���� �� ���
	if (isCleared)
	{
		delete g_GameState;
		g_GameState = nullptr;
	}
	if (Framework::instance().isKeyOn('q'))
	{
		if (g_GameState)
		{
			delete g_GameState;
			g_GameState = nullptr;
		}
		GameLib::Framework::instance().requestEnd();
	}
	if (Framework::instance().isEndRequested())
	{
		if (g_GameState)
		{
			delete g_GameState;
			g_GameState = nullptr;
		}
		return;
	}
}

namespace GameLib
{
	void Framework::update()
	{
		// 1��(1000 �и�������)�� ������ ������ ������ �� ��ŭ ����� ����ϸ� �������� �ִϸ��̼��� ǥ���� �� �ִ�.
		// �ٸ� ������� �ܼ��� ������ ���⸸ �ϴ� ���� ������ �̿��ؼ� �ִϸ��̼� �������� ������ �� �ִ�.
		GameLib::Framework::instance().sleep(1000 / 60);		
		MainLoop();
	}
}