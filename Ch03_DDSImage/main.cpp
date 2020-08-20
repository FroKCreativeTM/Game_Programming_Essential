#include "GameLib/Framework.h"
#include <fstream>
using namespace std;


void ReadFile(char** buffer, int* size, const char* fileName);
unsigned getUnsigned(const char* p);

bool				g_First = true;
int					g_ImageWidth = 0;
int					g_ImageHeight = 0;
unsigned*			g_ImageData = 0;

namespace GameLib
{
	void Framework::update()
	{
		// 만약 막 시작한 경우
		if (g_First)
		{
			g_First = false;
			char* buffer = nullptr;
			int size = 0;

			ReadFile(&buffer, &size, "bar.dds");

			g_ImageHeight = getUnsigned(&(buffer[12]));	// dwHeight
			g_ImageWidth = getUnsigned(&(buffer[16]));	// dwWidth
			g_ImageData = new unsigned[g_ImageWidth * g_ImageHeight];

			for (size_t i = 0; i < g_ImageWidth * g_ImageHeight; i++)
			{
				g_ImageData[i] = getUnsigned(&(buffer[128 + i * 4]));
			}
		}

		unsigned* vram = videoMemory();
		unsigned windowWidth = width();

		// 풀 이미지 출력
		// for (size_t y = 0; y < g_ImageHeight; y++)
		// {
		// 	for (size_t x = 0; x < g_ImageWidth; x++)
		// 	{
		// 		vram[y * windowWidth + x] = g_ImageData[y * g_ImageWidth + x];
		// 	}
		// }
		for (int y = g_ImageHeight - 32; y < g_ImageHeight; y++)
		{
			for (int x = g_ImageWidth - 32; x < g_ImageWidth; x++)
			{
				vram[y * windowWidth + x] = g_ImageData[y * g_ImageWidth + x];
			}
		}
	}
}

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

unsigned getUnsigned(const char* p)
{
	const unsigned char* up;
	up = reinterpret_cast<const unsigned char*>(p);
	unsigned r = up[0];

	// DDSURFACEDESC2 내용을 참고해서 그걸 이용해 정보를 r에 저장한다.
	// 자세한 부분은 책이나 MSDN 참조
	r |= up[1] << 8;
	r |= up[2] << 16;
	r |= up[3] << 24;

	return r;
}
