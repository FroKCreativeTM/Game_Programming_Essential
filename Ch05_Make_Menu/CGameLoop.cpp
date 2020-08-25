#include "Include.h"
#include "CGameLoop.h"
#include "GameMode.h"
#include "CSelectStage.h"
#include "CInGame.h"
#include "CMenu.h"
#include "CClear.h"
#include "CTitle.h"

CGameLoop::CGameLoop() : 
	m_eGameMode(GM_TITLE),
	m_isFirstGame(true),
	m_GameState(nullptr)
{
}

CGameLoop::~CGameLoop()
{
	SAFE_DELETE(m_GameState);
}

void CGameLoop::SelectStage()
{
	CSelectStage* selectStage = new CSelectStage();
	selectStage->SelectStage(m_GameStageName, m_eNextGameMode);

	if (m_eGameMode != m_eNextGameMode)
	{
		m_eGameMode = m_eNextGameMode;
		m_eNextGameMode = GM_NONE;
	}

	SAFE_DELETE(selectStage);
}

void CGameLoop::Title()
{
	CTitle* title = new CTitle();
	title->Draw(m_eGameMode);
	SAFE_DELETE(title);
}

void CGameLoop::Menu()
{
	CMenu* menu = new CMenu();
	menu->SelectMenu(m_isFirstGame, m_eGameMode, m_eNextGameMode);
	SAFE_DELETE(menu);
}

void CGameLoop::Clear()
{
	CClear* clear = new CClear();
	clear->Draw();

	if (m_eGameMode != m_eNextGameMode &&
		GameLib::Framework::instance().isKeyTriggered(' '))
	{
		m_eGameMode = m_eNextGameMode;
		m_eNextGameMode = GM_NONE;
	}

	SAFE_DELETE(clear);
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
		delete m_GameState;
		m_GameState = nullptr;
		m_eGameMode = GM_CLEAR;
		m_eNextGameMode = GM_MENU;
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
