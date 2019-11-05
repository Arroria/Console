#include "ConsoleDoubleBuffer.h"

#include <chrono>
int main()
{
	ConsoleDoubleBuffer cdb(40);
	int c = 0;
	while (true)
	{
		cdb.Clear();
		cdb.Begin();

		int y = c / 128;
		int x = c - y * 128;

		cdb.CursorTo(x, y);
		std::cout << "Hello, world!";
		
		
		cdb.End();
		cdb.Flipping();
		++c;

		Sleep(25);
	}
}
