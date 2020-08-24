#pragma once

// 2D 화면을 관리하기 위한 헬퍼 클래스입니다.
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
		// TODO: 여기에 반환 구문을 삽입합니다.
		return m_Array[h * m_nWidth + w];
	}

	const T& operator()(int w, int h) const 
	{
		// TODO: 여기에 반환 구문을 삽입합니다.
		return m_Array[h * m_nWidth + w];
	}
};

