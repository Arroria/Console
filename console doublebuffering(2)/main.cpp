#include "ConsoleDoubleBuffer.h"
int main()
{
	ConsoleDoubleBuffer cdb;

	cdb.Initialize();

	cdb.Clear();
	cdb.SetCursorPos(0, 0);
	std::cout << "123456789";
	cdb.Flipping();
	cdb.SetCursorPos(5, 0);
	std::cout << "123456789";
	cdb.Flipping();
	cdb.SetCursorPos(15, 0);
	std::cout << "123456789";
	cdb.Flipping();

	cdb.Release();
}
