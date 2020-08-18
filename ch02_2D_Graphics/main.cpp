#include <GameLib/2DGraphics1/src/Framework.h>

namespace GameLib
{
	void Framework::update()
	{
		unsigned* vram = Framework::instance().videoMemory();
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