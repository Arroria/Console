#pragma once
#include <Windows.h>
#include <string_view>

class ConsoleBuffer
{
public:
	ConsoleBuffer();
	~ConsoleBuffer();

	void Initialize(short sizeX = 80, short sizeY = 25);
	void Release();

	void Flipping();
	void Clear();


	void CursorMove(short x, short y);
	void CursorMove(COORD pos = { 0, 0 });

	void OutputText(std::string_view text) { OutputText(text, m_cursorPos); }
	void OutputText(std::string_view text, COORD pos);


private:
	HANDLE m_buffer[2];
	POINT m_size;
	int m_bid;

	COORD m_cursorPos;

	int _drawing() const { return m_bid; }
	int _showing() const { return m_bid ? 0 : 1; }
};

