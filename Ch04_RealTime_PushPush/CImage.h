#pragma once

#include "Include.h"
#include "CFileSystem.h"

class CImage
{
private : 
	int m_nWidth;
	int m_nHeight;
	unsigned* m_Data;

public : 
	CImage(const char* fileName)
	{
		CFileSystem f(fileName);
		m_nHeight = f.GetUnsigned(12);
		m_nWidth = f.GetUnsigned(16);
		m_Data = new unsigned[m_nWidth * m_nHeight];
		for (int i = 0; i < m_nWidth * m_nHeight; ++i) 
		{
			m_Data[i] = f.GetUnsigned(128 + i * 4);
		}
	}
	~CImage()
	{
		if (m_Data)
		{
			delete[] m_Data;
		}
		m_Data = nullptr;
	}

public: 
	void Draw(int dscX, int dscY, int srcX, int srcY, int _width, int _height) const
	{
		GameLib::Framework f = GameLib::Framework::instance();
		unsigned* vram = f.videoMemory();
		int nWindowWidth = f.width();

		/*
		// 가산 블렌딩 방식을 사용한다.
		// 공식 : aX + Y
		// 단점 : 다시 검은 색으로 바꿔주지 않는다면 새하얗게 된다.
		for (int y = 0; y < _height; y++)
		{
			for (int x = 0; x < _width; x++)
			{
				unsigned src = m_Data[(y + srcY) * m_nWidth + (x + srcX)];
				unsigned srcA = (src & 0xff000000) >> 24;
				unsigned srcR = (src & 0x00ff0000) >> 16;
				unsigned srcG = (src & 0x0000ff00) >> 8;
				unsigned srcB = (src & 0x000000ff);

				unsigned* dsc = &vram[(y + dscY) * nWindowWidth + (x + dscX)];
				unsigned dscR = (*dsc & 0x00ff0000) >> 16;
				unsigned dscG = (*dsc & 0x0000ff00) >> 8;
				unsigned dscB = (*dsc & 0x000000ff);

				unsigned R = srcR * srcA / 255 + dscR;
				unsigned G = srcG * srcA / 255 + dscG;
				unsigned B = srcB * srcA / 255 + dscB;

				// 범위 검사
				R = (R > 255) ? 255 : R;
				G = (G > 255) ? 255 : G;
				B = (B > 255) ? 255 : B;

				*dsc = (R << 16) | (G << 8) | B;
			}
		}
		*/

		// 알파 블렌딩
		// 
		for (int y = 0; y < _height; y++)
		{
			for (int x = 0; x < _width; x++)
			{
				unsigned src = m_Data[(y + srcY) * m_nWidth + (x + srcX)];
				unsigned srcA = (src & 0xff000000) >> 24;
				unsigned srcR = (src & 0x00ff0000);
				unsigned srcG = (src & 0x0000ff00);
				unsigned srcB = (src & 0x000000ff);

				unsigned* dsc = &vram[(y + dscY) * nWindowWidth + (x + dscX)];
				unsigned dscR = (*dsc & 0x00ff0000);
				unsigned dscG = (*dsc & 0x0000ff00);
				unsigned dscB = (*dsc & 0x000000ff);

				unsigned R = (srcR - dscR) * srcA / 255 + dscR;
				unsigned G = (srcG - dscG) * srcA / 255 + dscG;
				unsigned B = (srcB - dscB) * srcA / 255 + dscB;

				*dsc = (R & 0x00ff0000) | (G & 0x0000ff00) | B;
			}
		}
	}

public :
	int GetWidth() const { return m_nWidth; }
	int GetHeight() const { return m_nHeight; }
	const unsigned* GetData() const { return m_Data; }
};

