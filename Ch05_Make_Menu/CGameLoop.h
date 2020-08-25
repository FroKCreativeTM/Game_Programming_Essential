#pragma once

#include "Include.h"
#include "GameMode.h"
#include "CImage.h"
#include "CGameState.h"
#include "CInGame.h"

class CGameLoop
{
private :
	GameMode		m_eGameMode;				// ���� ��带 ��Ʈ�� �ϴ� enum ����
	GameMode		m_eNextGameMode;			// ���� ���� ��带 ��Ʈ�� �ϴ� enum ����
	std::string		m_GameStageName;			// ������ �������� �̸��� �����Ѵ�.
	bool			m_isFirstGame;				

	CGameState*		m_GameState = nullptr;		// ������ ���¸� �����ϱ� ���� Ŭ�����Դϴ�.
	const int		m_FrameInterval = 16;		// 16�и�������(60������)
	unsigned		m_PreviousTime[10];			// �� �ð��� �����صα� ���� �迭
	int				m_Counter = 0;				// �������� �󸶳� ����Ǿ����� �����Ѵ�.

public :
	CGameLoop();
	~CGameLoop();

public :
	void SelectStage();
	void Title();
	void Menu();
	void Clear();
	void InGame();

	GameMode GetGameMode() const;
	void SetGameMode(GameMode eGameMode);
};

