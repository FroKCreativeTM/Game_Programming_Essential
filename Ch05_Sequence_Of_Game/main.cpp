#include "Include.h"
#include "CState.h"
#include "CFileSystem.h"

/* ========== ���� ���� ========== */
CState* g_GameState = nullptr;		// ������ ���¸� �����ϱ� ���� Ŭ�����Դϴ�.
bool	g_IsGameStarted = false;	// ������ ���� ���� üũ
CImage* g_TitleImage = nullptr;		// ���� Ÿ��Ʋ

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
		SAFE_DELETE(g_GameState);
		g_IsGameStarted = false;
	}
}

void title()
{
	if (!g_TitleImage)
	{
		g_TitleImage = new CImage("title.dds");
	}
	if (GameLib::Framework::instance().isKeyOn(' '))
	{
		g_IsGameStarted = true;
		SAFE_DELETE(g_TitleImage);
		return;
	}
	g_TitleImage->Draw(0, 0, 0, 0, g_TitleImage->GetWidth(), g_TitleImage->GetHeight());
}

namespace GameLib
{
	void Framework::update()
	{
		if (!g_IsGameStarted)
		{
			title();
		}
		else
		{
			MainLoop();
		}

		if (Framework::instance().isKeyOn('q'))
		{
			GameLib::Framework::instance().requestEnd();
		}
		if (Framework::instance().isEndRequested())
		{
			SAFE_DELETE(g_GameState);
			SAFE_DELETE(g_TitleImage);
		}

		// 1��(1000 �и�������)�� ������ ������ ������ �� ��ŭ ����� ����ϸ� �������� �ִϸ��̼��� ǥ���� �� �ִ�.
		// �ٸ� ������� �ܼ��� ������ ���⸸ �ϴ� ���� ������ �̿��ؼ� �ִϸ��̼� �������� ������ �� �ִ�.
		GameLib::Framework::instance().sleep(1000 / 60);
	}
}