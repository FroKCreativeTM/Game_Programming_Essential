#include "Include.h"
#include "CImage.h"
#include "CTitle.h"
#include "GameMode.h"

CTitle::CTitle() : 
	m_titleImage(nullptr)
{
	m_titleImage = new CImage("image/title.dds");
}

CTitle::~CTitle()
{
	SAFE_DELETE(m_titleImage);
}

void CTitle::Draw(GameMode& eGameMode)
{
	m_titleImage->Draw(m_titleImage->GetWidth(), m_titleImage->GetHeight());

	if (GameLib::Framework::instance().isKeyOn(' '))
	{
		eGameMode = GM_MENU;
	}
}
