#pragma once

#include "Include.h"

// ���� �ý����� �����ϴ� Ŭ�����Դϴ�.
class CFileSystem
{
private : 
	char* m_Data;
	int m_FileSize;

private : 
	CFileSystem(const CFileSystem& ref) = delete;
	CFileSystem& operator=(const CFileSystem& ref) = delete;

public : 
	CFileSystem(const char* fileName) :
		m_Data(nullptr), m_FileSize(0)
	{
		std::ifstream in(fileName, std::ifstream::binary);
		if (!in)
		{
			m_Data = nullptr;
			m_FileSize = 0;
		}
		else
		{
			in.seekg(0, std::ifstream::end);
			m_FileSize = static_cast<int>(in.tellg());
			in.seekg(0, std::ifstream::beg);
			m_Data = new char[m_FileSize];
			in.read(m_Data, m_FileSize);
		}
		in.close();
	}
	~CFileSystem() 
	{
		if (m_Data)
		{
			delete[] m_Data;
		}
		m_Data = nullptr;
	}

	const char* GetData() { return m_Data; }
	int GetFileSize() const { return m_FileSize; }
	unsigned GetUnsigned(int p) const
	{	// dds�� ������ �������� �Լ�
		const unsigned char* up;
		up = reinterpret_cast<const unsigned char*>(m_Data);
		unsigned r = up[p];
		r |= up[p + 1] << 8;
		r |= up[p + 2] << 16;
		r |= up[p + 3] << 24;
		return r;
	}
};