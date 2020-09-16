#include "GameLib/Framework.h"

namespace GameLib
{
	void Framework::update()
	{
		// ���� �޸�
		// �� �����쿡 �޷��ִ� �׷��� ����̽�(�����쿡 ��ϵǾ��ִ� ���̷�Ʈ X�� ��ġ)�� �޸𸮿� ���� �����Ѵ�.
		unsigned* vram = Framework::instance().videoMemory();	// �׷��� ī�� == ���� ī��

		// �����쿡 ��ϵǾ��ִ� ����̽��� ���� ���̰�
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