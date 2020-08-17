#pragma once
class CFlag
{
private : 
	unsigned char m_cFlags;

public : 
	bool check(unsigned char f) const;
	void set(unsigned char f);
	void reset(unsigned char f);
};

