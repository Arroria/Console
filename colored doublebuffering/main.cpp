#include <iostream>
#include <string>
#include <chrono>
#include "ColoredDoubleBufferConsole.h"
ColoredDoubleBufferConsole cdbc;

void rendertest()
{
	std::array<int, 16> asdf;
	asdf.fill(0);
	while (true)
	{
		Sleep(33);
		int ran = rand() % 16;
		int v = asdf[ran]++;

		cdbc.SetColor(ran);
		cdbc.SetCursorPos(ran * 2, v);
		cdbc << "бс";
		cdbc.Flipping();
	}
}

int main()
{
	cdbc.Initialize();
	cdbc.Clear();
	rendertest();
	cdbc.Release();
}
