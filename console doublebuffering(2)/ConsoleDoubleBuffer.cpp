#include "ConsoleDoubleBuffer.h"

ConsoleDoubleBuffer::ConsoleDoubleBuffer()
	: m_stdOutputHandle(GetStdHandle(STD_OUTPUT_HANDLE))
	, m_cout_buffer(std::cout.rdbuf())
	, m_buffer()
	
	, m_bufferSize{ NULL, NULL }
{
}

ConsoleDoubleBuffer::~ConsoleDoubleBuffer()
{
}



bool ConsoleDoubleBuffer::Initialize()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(m_stdOutputHandle, &csbi))
		return false;

	return Initialize(csbi.dwSize.X, csbi.dwSize.Y);
}

bool ConsoleDoubleBuffer::Initialize(short x, short y)
{
	m_bufferSize = COORD{ x, y };
	_cout_catch_buffer();
	return true;
}

void ConsoleDoubleBuffer::Release()
{
	_cout_catch_cout();
}



void ConsoleDoubleBuffer::SetCursorPos(COORD pos)
{
	const std::streampos target = pos.X + (pos.Y * m_bufferSize.X);
	m_buffer.seekp(target);
	if (auto tellp = m_buffer.tellp(); tellp < target)
	{
		std::string str;
		str.resize(target - tellp, ' ');
		std::cout << str;
	}
}

void ConsoleDoubleBuffer::Clear()
{
	m_buffer.str("");
	m_buffer.clear();
}

void ConsoleDoubleBuffer::Flipping()
{
	_cout_catch_cout();
	SetConsoleCursorPosition(m_stdOutputHandle, COORD{ 0, 0 });
	SetCursorPos(0, 0);
	std::cout << m_buffer.str();
	_cout_catch_buffer();
}
