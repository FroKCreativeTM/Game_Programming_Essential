#pragma once

#include "Include.h"

// ���� �ý����� �����ϴ� Ŭ�����Դϴ�.
class CFileSystem
{
public : 
	CFileSystem() {}
	CFileSystem(const CFileSystem& ref) = delete;
	CFileSystem& operator=(const CFileSystem& ref) = delete;
	~CFileSystem() {}

	void ReadFile(char** buffer, int* size, const char* fileName)
	{
		ifstream in(fileName, ifstream::binary);
		if (!in)
		{
			*buffer = nullptr;
			*size = 0;
		}
		else
		{
			in.seekg(0, ifstream::end);
			*size = static_cast<int>(in.tellg());
			in.seekg(0, ifstream::beg);
			*buffer = new char[*size];
			in.read(*buffer, *size);
		}
		in.close();
	}
};