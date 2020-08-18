#include "Include.h"
#include "CState.h"

/* ========== ���� ���� ========== */
CState* g_GameState = nullptr;		// ������ ���¸� �����ϱ� ���� Ŭ�����Դϴ�.
CFileSystem fileSystem;

/* ========== ���� �Լ� ========== */
void MainLoop()
{
	// ���� ���� ���� Ŭ������ �������� ���� ���
	// �� ������ ���� ����̴�.
	if (!g_GameState)
	{
		// ���������� �����´�.
		const char* strStageName = "Stage1.txt";
		char* stageData = nullptr;
		int fileSize = 0;

		fileSystem.ReadFile(&stageData, &fileSize, strStageName);

		// ���� ���������� ������ �ʾҴٸ�
		if (!stageData)
		{
			MessageBox(nullptr, "�������� �ҷ����� ����", "�������� �ҷ����� ����", MB_OK);
			return;
		}

		// ���� ���� Ŭ������ �����Ѵ�.
		g_GameState = new CState(stageData, fileSize);

		// �ʿ� ������ �������� �����͸� �����Ѵ�.
		delete[] stageData;
		stageData = nullptr;

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

	cout << "a:left, d:right, w:up, s:down. command : ";
	char input;
	cin >> input;

	// �Է��� �������� ������Ʈ �Ѵ�.
	g_GameState->Update(input);
	// �̸� �������� �׸���.
	g_GameState->Draw();

	// ���� Ŭ���� �� ���
	if (isCleared)
	{
		MessageBox(nullptr, "You're Winner!", "Win", MB_OK);
		delete g_GameState;
		g_GameState = nullptr;

		GameLib::Framework::instance().requestEnd();
	}
}

namespace GameLib
{
	void Framework::update()
	{
		MainLoop();
	}
}