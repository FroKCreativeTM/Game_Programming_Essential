#include "IntArray2D.h"

IntArray2D::IntArray2D(int w, int h) : 
	m_nArray(nullptr), m_nWidth(w), m_nHeight(h)
{
	m_nArray = new int[m_nWidth * m_nHeight];
}

IntArray2D::~IntArray2D()
{
	delete[] m_nArray;
	m_nArray = nullptr;
}

int& IntArray2D::operator()(int w, int h)
{
	return m_nArray[h * m_nWidth + w];
}

const int& IntArray2D::operator()(int w, int h) const
{
	return m_nArray[h * m_nWidth + w];
}
