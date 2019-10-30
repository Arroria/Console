#include <Windows.h>
#include <iostream>
#include <string>
#include <chrono>
#include "ColoredDoubleBufferConsole.h"

int main()
{
	ColoredDoubleBufferConsole cdbc;
	std::cout << 1;

	cdbc.Initialize();




	" ";
	"0";
	" 0";
	"0 ";
	"";
	"";

	cdbc.Clear();
	cdbc.SetColor(1);
	cdbc.SetCursorPos(5, 0);
	cdbc << "123456789";
	cdbc.Flipping();
	cdbc.SetColor(5);
	cdbc.SetCursorPos(20, 0);
	cdbc << "123456789";
	cdbc.Flipping();
	cdbc.SetColor(10);
	cdbc.SetCursorPos(13, 0);
	cdbc << "123456789";
	cdbc.Flipping();

	cdbc.Release();
}
