#pragma once

#include <string>
#include "Include.h"
#include "CImage.h"

class CStringRenderer
{
private : 
	static CStringRenderer* m_Instance;
	CImage* m_StringImage;

public : 
	CStringRenderer(const char * fontImageFileName) : 
		m_StringImage(nullptr)
	{
		m_StringImage = new CImage(fontImageFileName);
	}
	CStringRenderer(CStringRenderer&) = delete;
	~CStringRenderer()
	{
		SAFE_DELETE(m_StringImage);
	}

public : 
	static void Create(const char* fontFileName)
	{
		STRONG_ASSERT(!m_Instance && "StringRenderer::create() called twice!");
		m_Instance = new CStringRenderer(fontFileName);
	}

	static void Destroy()
	{
		SAFE_DELETE(m_Instance);
	}

	static CStringRenderer* GetInstance()
	{
		return m_Instance;
	}

	// @Param
	// x, y : 위치
	// str : 내용
	// color : 색상 (기본값 : 하양색)
	void Draw(int x, int y, const char * str, unsigned color = 0xffffff)
	{
		// 폰트의 크기를 정한다.
		const int CHAR_WIDTH = 8;
		const int CHAR_HEIGHT = 16;

		// 위치와 크기를 매칭한다.
		int dstX = x * CHAR_WIDTH;
		int dstY = y * CHAR_HEIGHT;

		// 프레임워크 인스턴스를 가져온다.
		Framework f = Framework::instance();

		// y축 기준 있으면 안 되는 곳에 위치하면 종료
		if (dstY < 0 || dstY >= (int)f.height()) 
		{
			return;
		}

		// 폰트 출력
		for (int i = 0; str[i] != '\0'; ++i) 
		{
			// x 위치가 출력이 가능한 곳인 경우
			if (dstX >= 0 && (dstX + CHAR_WIDTH) < f.width()) 
			{
				// 아스키 범위 검사
				int t = str[i];
				if (t < 32 || t >= 128)	// 범위를 넘어가면 127로 결정
				{
					t = 127;
				}
				t -= 32;	// 왼쪽 위가 32이기 때문에 32 이동한다.
				int srcX = (t % 16) * CHAR_WIDTH;	// X는 이미지 폭으로 나눈 나머지
				int srcY = (t / 16) * CHAR_HEIGHT;	// Y는 이미지 폭으로 나눈 몫
				m_StringImage->DrawWithFixedColor(dstX, dstY, srcX, srcY, CHAR_WIDTH, CHAR_HEIGHT, color);
			}
			dstX += CHAR_WIDTH;	// 한 문자만큼 이동한다.
		}
	}
};

CStringRenderer* CStringRenderer::m_Instance = nullptr;

