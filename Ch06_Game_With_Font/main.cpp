#include "Include.h"
#include "CGameLoop.h"

/* ========== 전역 변수 ========== */
CGameLoop*			g_CGameLoop = nullptr;		// 게임 루프를 총괄하는 코어 클래스

void MainGame()
{
	if (!g_CGameLoop)
	{
		g_CGameLoop = new CGameLoop();
	}

	switch (g_CGameLoop->GetGameMode())
	{
	case GameMode::GM_TITLE:
		g_CGameLoop->Title();
		break;
	case GameMode::GM_SELECTSTAGE :
		g_CGameLoop->SelectStage();
		break;
	case GameMode::GM_MENU:
		g_CGameLoop->Menu();
		break;
	case GameMode::GM_INGAME:
		g_CGameLoop->InGame();
		break;
	case GameMode::GM_CLEAR :
		g_CGameLoop->Clear();
		break;
	}
}

namespace GameLib
{
	void Framework::update()
	{
		MainGame();

		if (GameLib::Framework::instance().isKeyOn('q'))
		{
			GameLib::Framework::instance().requestEnd();
		}
		if (GameLib::Framework::instance().isEndRequested())
		{
			SAFE_DELETE(g_CGameLoop);
		}
	}
}