#pragma once

#include "Include.h"
#include "ImageID.h"
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

		for (int y = 0; y < _height; y++)
		{
			for (int x = 0; x < _width; x++)
			{
				int pos = (y + dscY) * nWindowWidth + (x + dscX);
				unsigned* dsc = &vram[pos];
				*dsc = m_Data[(y + srcY) * m_nWidth + (x + srcX)];
			}
		}

	}

public :
	int GetWidth() const { return m_nWidth; }
	int GetHeight() const { return m_nHeight; }
	const unsigned* GetData() const { return m_Data; }
};

