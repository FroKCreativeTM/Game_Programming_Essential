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

	f.sleep(1);	// 1 밀리초씩만 재워서 CPU를 이 게임이 모두 점유하는 건 막자..

	// 가변 프레임을 구현하기 위한 변수를 선언한다.
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

	// 아직 게임 상태 클래스가 생성되지 않은 경우
	// 즉 게임이 켜진 경우이다.
	if (!m_GameState)
	{
		// 파일 시스템을 불러온다.
		CFileSystem fileSystem(m_GameStageName);

		// 만약 스테이지가 읽히지 않았다면
		if (!fileSystem.GetData())
		{
			GameLib::cout << "Stage file could not be read" << GameLib::endl;
			return;
		}

		// 게임 상태 클래스를 생성한다.
		m_GameState = new CGameState(fileSystem.GetData(), fileSystem.GetFileSize());

		// 그린다.
		m_GameState->Draw();
		return; // 함수를 종료한다.
	}

	// 클리어 여부를 검사하기 전에 클리어 여부 변수를 생성한다.
	bool isCleared = false;

	if (m_GameState->checkClear())
	{
		isCleared = true;
	}

	// 입력을 바탕으로 업데이트 한다.
	m_GameState->Update(frameInterval);
	// 이를 바탕으로 그린다.
	m_GameState->Draw();

	// 만약 클리어 한 경우
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
