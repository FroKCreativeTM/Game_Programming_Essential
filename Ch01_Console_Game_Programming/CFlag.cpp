#include "CFlag.h"

bool CFlag::check(unsigned char f) const
{
	unsigned char t = m_cFlags;
	t >>= f;
	t <<= 7;

	return (t != 0) ? true : false;
}

void CFlag::set(unsigned char f)
{
	m_cFlags |= f;
}

void CFlag::reset(unsigned char f)
{
	// m_cFlags &= (f ^ 255);
	m_cFlags &= ~f;
}