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
	// x, y : ��ġ
	// str : ����
	// color : ���� (�⺻�� : �Ͼ��)
	void Draw(int x, int y, const char * str, unsigned color = 0xffffff)
	{
		// ��Ʈ�� ũ�⸦ ���Ѵ�.
		const int CHAR_WIDTH = 8;
		const int CHAR_HEIGHT = 16;

		// ��ġ�� ũ�⸦ ��Ī�Ѵ�.
		int dstX = x * CHAR_WIDTH;
		int dstY = y * CHAR_HEIGHT;

		// �����ӿ�ũ �ν��Ͻ��� �����´�.
		Framework f = Framework::instance();

		// y�� ���� ������ �� �Ǵ� ���� ��ġ�ϸ� ����
		if (dstY < 0 || dstY >= (int)f.height()) 
		{
			return;
		}

		// ��Ʈ ���
		for (int i = 0; str[i] != '\0'; ++i) 
		{
			// x ��ġ�� ����� ������ ���� ���
			if (dstX >= 0 && (dstX + CHAR_WIDTH) < f.width()) 
			{
				// �ƽ�Ű ���� �˻�
				int t = str[i];
				if (t < 32 || t >= 128)	// ������ �Ѿ�� 127�� ����
				{
					t = 127;
				}
				t -= 32;	// ���� ���� 32�̱� ������ 32 �̵��Ѵ�.
				int srcX = (t % 16) * CHAR_WIDTH;	// X�� �̹��� ������ ���� ������
				int srcY = (t / 16) * CHAR_HEIGHT;	// Y�� �̹��� ������ ���� ��
				m_StringImage->DrawWithFixedColor(dstX, dstY, srcX, srcY, CHAR_WIDTH, CHAR_HEIGHT, color);
			}
			dstX += CHAR_WIDTH;	// �� ���ڸ�ŭ �̵��Ѵ�.
		}
	}
};

CStringRenderer* CStringRenderer::m_Instance = nullptr;

