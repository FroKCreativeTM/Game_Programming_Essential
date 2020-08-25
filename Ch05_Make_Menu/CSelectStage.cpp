#include <sstream>
#include "CSelectStage.h"
#include "GameMode.h"

CSelectStage::CSelectStage() : 
	m_selectStage(nullptr)
{
	m_selectStage = new CImage("image/stageSelect.dds");
}

CSelectStage::~CSelectStage()
{
	SAFE_DELETE(m_selectStage);
}

void CSelectStage::SelectStage(std::string& strGameStage, GameMode& eNextGameMode)
{
	int stage = 0;
	GameLib::Framework f = GameLib::Framework::instance();

	m_selectStage->Draw(m_selectStage->GetWidth(), m_selectStage->GetHeight());

	char numberChars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	for (int i = 0; i < 10; ++i) {
		if (f.isKeyTriggered(numberChars[i])) {
			stage = i;
		}
	}

	if (stage != 0)
	{
		std::ostringstream oss;
		oss << "stageData/" << stage << ".txt";
		strGameStage = oss.str();
		eNextGameMode = GM_INGAME;
	}
}
