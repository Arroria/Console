#include "TetrisGame.h"
#include "../console doublebuffering(2)/ConsoleDoubleBuffer.h"
int main()
{
	ConsoleDoubleBuffer cdb;
	cdb.Initialize();

	TetrisGame tgame(10, 20);
	while (true)
	{
		tgame.Update();
		tgame.Render(cdb);
	}

	cdb.Release();
	return 0;
}

void RenderDefaultTchunkHub(ConsoleDoubleBuffer& cdb)
{
	cdb.Clear();
	const auto& hub = _default_tetris_tchunk_hub();
	for (size_t hubI = 0; hubI < hub.size(); ++hubI)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t _backY = 0; _backY < 4; _backY++)
			{
				cdb.SetCursorPos(i * 9, hubI * 5 + _backY);
				std::cout << "брбрбрбр";
			}

			auto& data = hub[hubI]->data(i);
			for (auto iter = data.begin(); iter != data.end(); ++iter)
			{
				cdb.SetCursorPos(iter->x * 2 + i * 9, (3 - iter->y) + hubI * 5);
				std::cout << "бс";
			}
		}
	}
	cdb.Flipping();
}