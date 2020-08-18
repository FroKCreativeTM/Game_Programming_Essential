#pragma once

// 2D 화면을 관리하기 위한 헬퍼 클래스입니다.
template <typename T>
class Array2D
{
private : 
	T* m_Array;
	int m_nWidth;
	int m_nHeight;

public : 
	Array2D();
	~Array2D();

public :
	void SetSize(int w, int h);
	T& operator()(int w, int h);
	const T& operator()(int w, int h) const;
};

