#include <Windows.h>
#include "ConsoleBuffer.h"

#include <iostream>
#include <string>
using std::cout;
using std::endl;

int main()
{
	ConsoleBuffer cb;
	cb.Initialize();

	while (true)
	{
		static int n = 0;
		n++;

		cb.Clear();
		cb.CursorMove();

		if (n % 2)
		for (size_t i = 0; i < 20; i++)
		{
			for (size_t j = 0; j < 20; j++)
			{
				cb.OutputText("YO");
			}
			cb.CursorMove(0, i);
		}
		cb.OutputText(std::to_string(n));

		cb.Flipping();
	}

	cb.Release();
 }
