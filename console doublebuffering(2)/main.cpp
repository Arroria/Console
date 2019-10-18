#include "ConsoleDoubleBuffer.h"
int main()
{
	ConsoleDoubleBuffer cdb;

	cdb.Initialize();

	cdb.Clear();
	cdb.SetCursorPos(0, 0);
	std::cout << "1234567890";
	cdb.Flipping();

	cdb.Clear();
	cdb.SetCursorPos(0, 0);
	std::cout << "우효www";
	cdb.Flipping();

	//while (!GetAsyncKeyState(VK_ESCAPE))
	//{
	//	cdb.Clear();
	//	static int a = 0;	++a;
	//	cdb.SetCursorPos(a, 1);
	//	std::cout << "우효www" << a;
	//	cdb.Flipping();
	//}

	cdb.Release();
}
