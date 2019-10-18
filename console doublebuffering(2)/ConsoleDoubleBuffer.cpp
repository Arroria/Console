#include "ConsoleDoubleBuffer.h"

ConsoleDoubleBuffer::ConsoleDoubleBuffer()
	: m_stdOutputHandle(GetStdHandle(STD_OUTPUT_HANDLE))
	, m_cout_buffer(std::cout.rdbuf())
	, m_buffer()
	
	, m_bufferCoordSize{ NULL, NULL }
	, m_prevFlippedBufferLength(0)
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
	m_bufferCoordSize = COORD{ x, y };
	_cout_catch_buffer();
	return true;
}

void ConsoleDoubleBuffer::Release()
{
	_cout_catch_cout();
}



void ConsoleDoubleBuffer::SetCursorPos(COORD pos)
{
	const std::streampos target = size_t(pos.X) + size_t(pos.Y) * size_t(m_bufferCoordSize.X);
	if (m_buffer.seekp(target).fail())
	{
		m_buffer.clear();
		m_buffer.seekp(0, std::ios::end);
		if (auto tellp = m_buffer.tellp(); tellp < target)
		{
			std::string str;
			str.resize(target - tellp, ' ');
			std::cout << str;
		}
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

	size_t bufferSize = _get_buffer_size();
	if (m_prevFlippedBufferLength > bufferSize)
		std::cout << std::string(m_prevFlippedBufferLength - bufferSize, ' ');
	m_prevFlippedBufferLength = bufferSize;

	_cout_catch_buffer();
}
