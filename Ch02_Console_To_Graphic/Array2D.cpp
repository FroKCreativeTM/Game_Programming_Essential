#include "Array2D.h"

template<typename T>
Array2D<T>::Array2D() :
	m_Array(nullptr), m_nWidth(0), m_nHeight(0)
{
}


template<typename T>
inline Array2D<T>::~Array2D()
{
	delete[] m_Array;
	m_Array = nullptr;
}

template<typename T>
inline void Array2D<T>::SetSize(int w, int h)
{
	m_nWidth = w;
	m_nHeight = h;
	m_Array = new T[m_nWidth * m_nHeight];
}

template<typename T>
inline T& Array2D<T>::operator()(int w, int h)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_Array[h * m_nWidth + w];
}

template<typename T>
inline const T& Array2D<T>::operator()(int w, int h) const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_Array[h * m_nWidth + w];
}
