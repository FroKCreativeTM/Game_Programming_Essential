#pragma once

#include "Include.h"
#include "GameMode.h"
#include "CImage.h"
#include "CGameState.h"

class CGameLoop
{
private : 
	CGameState*		m_GameState = nullptr;		// 게임의 상태를 관리하기 위한 클래스입니다.
	const int		m_FrameInterval = 16;		// 16밀리세컨드(60프레임)
	unsigned		m_PreviousTime[10];			// 전 시간을 저장해두기 위한 배열
	int				m_Counter = 0;				// 프레임이 얼마나 진행되었나를 저장한다.
	CImage*			m_LoopImage = nullptr;		// 각각 모드마다 출력할 이미지
	GameMode		m_eGameMode;				// 게임 모드를 컨트롤 하는 enum 변수
	std::string		m_GameStageName;			// 게임의 스테이지 이름을 저장한다.
	bool			m_isFirstGame;				

public :
	CGameLoop();
	~CGameLoop();

public :
	void SelectMenu();
	void Title();
	void Menu();
	void Clear();
	void InGame();

	GameMode GetGameMode() const;
	void SetGameMode(GameMode eGameMode);
};

