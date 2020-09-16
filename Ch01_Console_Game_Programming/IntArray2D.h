#pragma once


class IntArray2D
{
private : 
	int* m_nArray;
	const int m_nWidth;
	const int m_nHeight;

public : 
	IntArray2D(int w, int h);
	~IntArray2D();
	int& operator()(int w, int h);
	const int& operator()(int w, int h) const;
};

