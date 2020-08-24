#include "Include.h"
#include "CGameLoop.h"
#include "GameMode.h"
#include "CImage.h"

CGameLoop::CGameLoop() : 
	m_GameState(nullptr),
	m_eGameMode(GM_TITLE),
	m_isFirstGame(true)
{
}

CGameLoop::~CGameLoop()
{
	SAFE_DELETE(m_GameState);
	SAFE_DELETE(m_LoopImage);
}

void CGameLoop::SelectMenu()
{
	GameLib::Framework f = GameLib::Framework::instance();

	m_LoopImage = new CImage("image/stageSelect.dds");
	m_LoopImage->Draw(m_LoopImage->GetWidth(), m_LoopImage->GetHeight());

	if (f.isKeyOn('1'))
	{
		m_GameStageName = "stageData/1.txt";
	}
	else if (f.isKeyOn('2'))
	{
		m_GameStageName = "stageData/2.txt";
	}
	else if (f.isKeyOn('3'))
	{
		m_GameStageName = "stageData/3.txt";
	}
	else if (f.isKeyOn('4'))
	{
		m_GameStageName = "stageData/4.txt";
	}
	else if (f.isKeyOn('5'))
	{
		m_GameStageName = "stageData/5.txt";
	}
	else if (f.isKeyOn('6'))
	{
		m_GameStageName = "stageData/6.txt";
	}
	else if (f.isKeyOn('7'))
	{
		m_GameStageName = "stageData/7.txt";
	}
	else if (f.isKeyOn('8'))
	{
		m_GameStageName = "stageData/8.txt";
	}
	else if (f.isKeyOn('9'))
	{
		m_GameStageName = "stageData/9.txt";
	}

	SAFE_DELETE(m_LoopImage);

	SetGameMode(GM_INGAME);
}

void CGameLoop::Title()
{
	if (!m_LoopImage)
	{
		m_LoopImage = new CImage("image/title.dds");
	}

	m_LoopImage->Draw(m_LoopImage->GetWidth(), m_LoopImage->GetHeight());

	if (GameLib::Framework::instance().isKeyOn(' '))
	{
		m_eGameMode = GM_MENU;
		SAFE_DELETE(m_LoopImage);
	}
}

void CGameLoop::Menu()
{
	GameLib::Framework f = GameLib::Framework::instance();

	if (!m_LoopImage)
	{
		m_LoopImage = new CImage("image/menu.dds");
	}
	m_LoopImage->Draw(m_LoopImage->GetWidth(), m_LoopImage->GetHeight());

	if (f.isKeyOn('a') && !m_isFirstGame)
	{
		m_eGameMode = GM_INGAME;
	}
	else if (f.isKeyOn('s'))
	{
		m_eGameMode = GM_SELECTSTAGE;
	}
	else if (f.isKeyOn('d'))
	{
		m_eGameMode = GM_TITLE;
	}
	else if (f.isKeyOn('f'))
	{
		GameLib::Framework::instance().requestEnd();
	}
		
	SAFE_DELETE(m_LoopImage);
}

void CGameLoop::Clear()
{
	if (!m_LoopImage)
	{
		m_LoopImage = new CImage("image/clear.dds");
	}
	m_LoopImage->Draw(m_LoopImage->GetWidth(), m_LoopImage->GetHeight());

	if (GameLib::Framework::instance().isKeyOn(' '))
	{
		m_eGameMode = GM_MENU;
		SAFE_DELETE(m_LoopImage);
		return;
	}
}

void CGameLoop::InGame()
{
	Framework f = Framework::instance();

	

	f.sleep(1);	// 1 �и��ʾ��� ����� CPU�� �� ������ ��� �����ϴ� �� ����..

	// ���� �������� �����ϱ� ���� ������ �����Ѵ�.
	unsigned currentTime = f.time();
	unsigned frameInterval = currentTime - m_PreviousTime[9];
	unsigned frameInterval10 = currentTime - m_PreviousTime[0];

	if (m_Counter % 60 == 0)
	{
		GameLib::cout << "FrameInterval : " << frameInterval10 / 10;
		GameLib::cout << " FrameRate : " << 10 * 1000 / frameInterval10 << GameLib::endl;
	}
	m_Counter++;

	for (int i = 0; i < 9; i++)
	{
		m_PreviousTime[i] = m_PreviousTime[i + 1];
	}
	m_PreviousTime[9] = currentTime;

	// ���� ���� ���� Ŭ������ �������� ���� ���
	// �� ������ ���� ����̴�.
	if (!m_GameState)
	{
		// ���� �ý����� �ҷ��´�.
		CFileSystem fileSystem(m_GameStageName);

		// ���� ���������� ������ �ʾҴٸ�
		if (!fileSystem.GetData())
		{
			GameLib::cout << "Stage file could not be read" << GameLib::endl;
			return;
		}

		// ���� ���� Ŭ������ �����Ѵ�.
		m_GameState = new CGameState(fileSystem.GetData(), fileSystem.GetFileSize());

		// �׸���.
		m_GameState->Draw();
		m_isFirstGame = false;
		return; // �Լ��� �����Ѵ�.
	}

	// Ŭ���� ���θ� �˻��ϱ� ���� Ŭ���� ���� ������ �����Ѵ�.
	bool isCleared = false;

	if (m_GameState->checkClear())
	{
		isCleared = true;
	}

	// �Է��� �������� ������Ʈ �Ѵ�.
	m_GameState->Update(frameInterval);
	// �̸� �������� �׸���.
	m_GameState->Draw();

	// ���� Ŭ���� �� ���
	if (isCleared)
	{
		m_eGameMode = GM_CLEAR;
		delete m_GameState;
		m_GameState = nullptr;
	}
	if (Framework::instance().isKeyOn('q'))
	{
		GameLib::Framework::instance().requestEnd();
	}
	if (Framework::instance().isEndRequested())
	{
		SAFE_DELETE(m_GameState);
		return;
	}
}

GameMode CGameLoop::GetGameMode() const
{
	return m_eGameMode;
}

void CGameLoop::SetGameMode(GameMode eGameMode)
{
	m_eGameMode = eGameMode;
}
