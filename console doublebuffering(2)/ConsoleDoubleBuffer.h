#pragma once
#include <Windows.h>
// std::ostream::tie와 std::ostream::rdbuf에 대해 = https://digitz.tistory.com/12
#include <iostream>
#include <sstream>
#include <string>

class ConsoleDoubleBuffer
{
public:
	static constexpr double _streambuf_expansion_magnification = 1.5;

	ConsoleDoubleBuffer(size_t widthLimite);
	~ConsoleDoubleBuffer();


	void Begin();
	void End();

	void CursorTo(POINT pos) { return CursorTo(pos.x, pos.y); }
	void CursorTo(size_t x, size_t y);

	void Clear();
	void Flipping();


private:
	const size_t m_widthLimite;
	std::ostringstream m_cdbStream;
	std::streambuf* m_coutStreambuf;

	bool m_isRun;
	size_t m_prevFlippedDataLength;


	void _set_cout_streambuf_cdb()	{ std::cout.rdbuf(m_cdbStream.rdbuf()); }
	void _set_cout_streambuf_cout()	{ std::cout.rdbuf(m_coutStreambuf); }
};
