#include "ConsoleDoubleBuffer.h"
int main()
{
	ConsoleDoubleBuffer cdb;

	cdb.Initialize();

	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		cdb.Clear();
		static int a = 0;	++a;
		cdb.SetCursorPos(a, 1);
		std::cout << "¿ìÈ¿www" << a;
		cdb.Flipping();
	}

	cdb.Release();
}
