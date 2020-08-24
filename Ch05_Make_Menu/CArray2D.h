#pragma once

// 2D ȭ���� �����ϱ� ���� ���� Ŭ�����Դϴ�.
template <typename T>
class CArray2D
{
private : 
	T* m_Array;
	int m_nWidth;
	int m_nHeight;

public : 
	CArray2D() :
		m_Array(nullptr), m_nWidth(0), m_nHeight(0)
	{

	}
	~CArray2D()
	{
		delete[] m_Array;
		m_Array = nullptr;
	}

public :
	void SetSize(int w, int h) 
	{
		m_nWidth = w;
		m_nHeight = h;
		m_Array = new T[w * h];
	}

	T& operator()(int w, int h) 
	{
		// TODO: ���⿡ ��ȯ ������ �����մϴ�.
		return m_Array[h * m_nWidth + w];
	}

	const T& operator()(int w, int h) const 
	{
		// TODO: ���⿡ ��ȯ ������ �����մϴ�.
		return m_Array[h * m_nWidth + w];
	}
};

