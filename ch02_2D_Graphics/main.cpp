#include "GameLib/Framework.h"

namespace GameLib
{
	void Framework::update()
	{
		// 비디오 메모리
		// 즉 윈도우에 달려있는 그래픽 디바이스(윈도우에 등록되어있는 다이렉트 X의 장치)의 메모리에 직접 접근한다.
		unsigned* vram = Framework::instance().videoMemory();	// 그래픽 카드 == 비디오 카드

		// 윈도우에 등록되어있는 디바이스의 가로 길이값
		int width = Framework::instance().width();

		for (size_t i = 100; i <= 200; i++)
		{
			for (size_t j = 100; j <= 200; j++)
			{
				vram[j * width + i] = 0xff0000;
			}
		}
	}
}