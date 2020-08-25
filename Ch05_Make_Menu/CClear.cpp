#include "CClear.h"

CClear::CClear()
	: m_clearImage(nullptr)
{
	m_clearImage = new CImage("image/clear.dds");
}

CClear::~CClear()
{
	SAFE_DELETE(m_clearImage);
}

void CClear::Draw()
{
	m_clearImage->Draw(m_clearImage->GetWidth(), m_clearImage->GetHeight());
}
