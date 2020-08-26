#include <sstream>
#include "CSelectStage.h"
#include "GameMode.h"
#include "CStringRenderer.h"

CSelectStage::CSelectStage() : 
	m_selectStage(nullptr)
{
	m_selectStage = new CImage("data/image/dummy.dds");
}

CSelectStage::~CSelectStage()
{
	SAFE_DELETE(m_selectStage);
}

void CSelectStage::SelectStage(std::string& strGameStage, GameMode& eNextGameMode)
{
	int stage = 0;
	GameLib::Framework f = GameLib::Framework::instance();

	if (f.isKeyTriggered('w')) 
	{
		--m_nCursorPosition;
		if (m_nCursorPosition < 1 - 1) { //?イナスは最大値にル?プ
			m_nCursorPosition = 9 - 1;
		}
	}
	else if (f.isKeyTriggered('s')) {
		++m_nCursorPosition;
		if (m_nCursorPosition > 9 - 1) { //9面を越えたら1面にル?プ
			m_nCursorPosition = 1 - 1;
		}
	}
	else if (f.isKeyTriggered(' ')) {
		stage = m_nCursorPosition + 1;
	}

	m_selectStage->Draw(m_selectStage->GetWidth(), m_selectStage->GetHeight());
	CStringRenderer* sr = CStringRenderer::GetInstance();
	sr->Draw(0, 0, "STAGE SELECTION");

	char numberChars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	for (int i = 0; i < 10; ++i) {
		if (f.isKeyTriggered(numberChars[i])) {
			stage = i;
		}
	}

	if (stage != 0)
	{
		std::ostringstream oss;
		oss << "data/stageData/" << stage << ".txt";
		strGameStage = oss.str();
		eNextGameMode = GameMode::GM_INGAME;
	}

	sr->Draw(0, m_nCursorPosition + 1, ">");
}
