#include "CClear.h"
#include "CStringRenderer.h"

CClear::CClear()
	: m_clearImage(nullptr)
{
	m_clearImage = new CImage("data/image/dummy.dds");
}

CClear::~CClear()
{
	SAFE_DELETE(m_clearImage);
}

void CClear::Draw()
{
	m_clearImage->Draw(m_clearImage->GetWidth(), m_clearImage->GetHeight());
	CStringRenderer::GetInstance()->Draw(0, 0, "CLEAR!");
}
