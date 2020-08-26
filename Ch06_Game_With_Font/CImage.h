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
	CImage(const std::string& fileName)
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

		// ¾ËÆÄ ºí·»µù
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

	void Draw(int _width, int _height) const
	{
		GameLib::Framework f = GameLib::Framework::instance();
		unsigned* vram = f.videoMemory();
		int nWindowWidth = f.width();


		// ¾ËÆÄ ºí·»µù
		// 
		for (int y = 0; y < _height; y++)
		{
			for (int x = 0; x < _width; x++)
			{
				unsigned src = m_Data[y * m_nWidth + x];
				unsigned srcA = (src & 0xff000000) >> 24;
				unsigned srcR = (src & 0x00ff0000);
				unsigned srcG = (src & 0x0000ff00);
				unsigned srcB = (src & 0x000000ff);

				unsigned* dsc = &vram[y * nWindowWidth + x];
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

	void DrawWithFixedColor(int dscX, int dscY, int srcX, int srcY, int _width, int _height, unsigned color) const
	{
		unsigned* vram = Framework::instance().videoMemory();
		int windowWidth = Framework::instance().width();

		unsigned srcR = color & 0xff0000;
		unsigned srcG = color & 0x00ff00;
		unsigned srcB = color & 0x0000ff;

		for (int y = 0; y < _height; ++y)
		{
			for (int x = 0; x < _width; ++x)
			{
				unsigned src = m_Data[(y + srcY) * m_nWidth + (x + srcX)];
				unsigned* dst = &vram[(y + dscY) * windowWidth + (x + dscX)];
				unsigned srcA = (src & 0xff000000) >> 24;
				unsigned dstR = *dst & 0xff0000;
				unsigned dstG = *dst & 0x00ff00;
				unsigned dstB = *dst & 0x0000ff;
				unsigned r = (srcR - dstR) * srcA / 255 + dstR;
				unsigned g = (srcG - dstG) * srcA / 255 + dstG;
				unsigned b = (srcB - dstB) * srcA / 255 + dstB;
				*dst = (r & 0xff0000) | (g & 0x00ff00) | b;
			}
		}
	}

public :
	int GetWidth() const { return m_nWidth; }
	int GetHeight() const { return m_nHeight; }
	const unsigned* GetData() const { return m_Data; }
};

