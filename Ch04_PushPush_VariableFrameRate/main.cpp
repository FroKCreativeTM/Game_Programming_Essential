#include "Include.h"
#include "CState.h"
#include "CFileSystem.h"

/* ========== 전역 변수 ========== */
CState* g_GameState = nullptr;		// 게임의 상태를 관리하기 위한 클래스입니다.
const int gFrameInterval = 16;		// 16밀리세컨드(60프레임)
unsigned gPreviousTime[10];			// 전 시간을 저장해두기 위한 배열
int gCounter = 0;					// 프레임이 얼마나 진행되었나를 저장한다.


/* ========== 전역 함수 ========== */
void MainLoop()
{
	Framework f = Framework::instance();

	f.sleep(1);	// 1 밀리초씩만 재워서 CPU를 이 게임이 모두 점유하는 건 막자..

	// 가변 프레임을 구현하기 위한 변수를 선언한다.
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
	g_GameState->Update(frameInterval);
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