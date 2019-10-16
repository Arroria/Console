#pragma once
#include <Windows.h>
// std::ostream::tie와 std::ostream::rdbuf에 대해 = https://digitz.tistory.com/12
#include <iostream>
#include <sstream>
#include <string>

class ConsoleDoubleBuffer
{
public:
	ConsoleDoubleBuffer();
	~ConsoleDoubleBuffer();

	bool Initialize();
	bool Initialize(short x, short y);
	void Release();

	void SetCursorPos(short x, short y) { return SetCursorPos(COORD{ x, y }); }
	void SetCursorPos(COORD pos);

	void Clear();
	void Flipping();



private:
	// Buffer
	const HANDLE m_stdOutputHandle;
	std::streambuf* m_cout_buffer;
	std::ostringstream m_buffer;

	COORD m_bufferSize;


	void _cout_catch_buffer() { std::cout.rdbuf(m_buffer.rdbuf()); }
	void _cout_catch_cout() { std::cout.rdbuf(m_cout_buffer); }
};
