#pragma once

// 2D ȭ���� �����ϱ� ���� ���� Ŭ�����Դϴ�.
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

