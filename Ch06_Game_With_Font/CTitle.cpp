#include "Include.h"
#include "CImage.h"
#include "CTitle.h"
#include "GameMode.h"
#include "CStringRenderer.h"

CTitle::CTitle() : 
	m_titleImage(nullptr)
{
	m_titleImage = new CImage("data/image/dummy.dds");
}

CTitle::~CTitle()
{
	SAFE_DELETE(m_titleImage);
}

void CTitle::Draw(GameMode& eGameMode)
{
	if (GameLib::Framework::instance().isKeyOn(' '))
	{
		eGameMode = GameMode::GM_MENU;
	}
	m_titleImage->Draw(m_titleImage->GetWidth(), m_titleImage->GetHeight());
	CStringRenderer::GetInstance()->Draw(0, 0, "TITLE: Nimotsu Kun.");
	CStringRenderer::GetInstance()->Draw(0, 1, "PRESS SPACE KEY");
}
