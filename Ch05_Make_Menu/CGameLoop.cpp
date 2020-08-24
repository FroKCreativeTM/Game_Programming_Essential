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
		m_isFirstGame = false;
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
