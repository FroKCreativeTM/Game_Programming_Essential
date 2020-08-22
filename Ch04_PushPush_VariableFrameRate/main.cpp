#include "Include.h"
#include "CState.h"
#include "CFileSystem.h"

/* ========== 전역 변수 ========== */
CState* g_GameState = nullptr;		// 게임의 상태를 관리하기 위한 클래스입니다.


/* ========== 전역 함수 ========== */
void MainLoop()
{
	// 아직 게임 상태 클래스가 생성되지 않은 경우
	// 즉 게임이 켜진 경우이다.
	if (!g_GameState)
	{
		// 스테이지를 가져온다.
		const char* strStageName = "Stage1.txt";

		// 파일 시스템을 불러온다.
		CFileSystem fileSystem(strStageName);

		// 만약 스테이지가 읽히지 않았다면
		if (!fileSystem.GetData())
		{
			GameLib::cout << "Stage file could not be read" << GameLib::endl;
			return;
		}

		// 게임 상태 클래스를 생성한다.
		g_GameState = new CState(fileSystem.GetData(), fileSystem.GetFileSize());

		// 그린다.
		g_GameState->Draw();
		return; // 함수를 종료한다.
	}

	// 클리어 여부를 검사하기 전에 클리어 여부 변수를 생성한다.
	bool isCleared = false;

	if (g_GameState->checkClear())
	{
		isCleared = true;
	}


	// 입력을 바탕으로 업데이트 한다.
	g_GameState->Update();
	// 이를 바탕으로 그린다.
	g_GameState->Draw();

	// 만약 클리어 한 경우
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
		// 1초(1000 밀리세컨드)에 프레임 값으로 나누고 그 만큼 재워서 사용하면 스무스한 애니메이션을 표현할 수 있다.
		// 다른 방법으로 단순히 루프를 돌기만 하는 비지 루프를 이용해서 애니메이션 프레임을 조절할 수 있다.
		GameLib::Framework::instance().sleep(1000 / 60);		
		MainLoop();
	}
}