#include "Include.h"
#include "CState.h"
#include "CFileSystem.h"

/* ========== ���� ���� ========== */
CState* g_GameState = nullptr;		// ������ ���¸� �����ϱ� ���� Ŭ�����Դϴ�.


/* ========== ���� �Լ� ========== */
void MainLoop()
{
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
	g_GameState->Update();
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
		MainLoop();
	}
}