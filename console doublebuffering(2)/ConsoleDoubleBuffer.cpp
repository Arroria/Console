#include "ConsoleDoubleBuffer.h"

void __add_void_string(size_t length) { std::cout << std::string(length, ' '); }



ConsoleDoubleBuffer::ConsoleDoubleBuffer(size_t widthLimite)
	: m_widthLimite(widthLimite)
	, m_cdbStream()
	, m_coutStreambuf(std::cout.rdbuf())
	
	, m_isRun(false)
	, m_prevFlippedDataLength{ NULL, NULL }
{
	COORD size = { widthLimite, 9000 };

	m_front_buffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	m_back_buffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleScreenBufferSize(m_front_buffer, size);
	SetConsoleScreenBufferSize(m_back_buffer, size);
	SetConsoleActiveScreenBuffer(m_front_buffer);
}

ConsoleDoubleBuffer::~ConsoleDoubleBuffer()
{
	CloseHandle(m_front_buffer);
	CloseHandle(m_back_buffer);
	if (m_isRun)
		_set_cout_streambuf_cout();
}



void ConsoleDoubleBuffer::Begin()
{
	_set_cout_streambuf_cdb();
	m_isRun = true;
}

void ConsoleDoubleBuffer::End()
{
	_set_cout_streambuf_cout();
	m_isRun = false;
}




void ConsoleDoubleBuffer::CursorTo(size_t x, size_t y)
{
	if (!m_isRun)
		return;

	using pos_t = long long;

	pos_t target = m_widthLimite * y + x;
	if (m_cdbStream.seekp(target).fail())
	{
		m_cdbStream.clear();
		m_cdbStream.seekp(0, std::ios::end);
		pos_t nowPos = m_cdbStream.tellp();
		
		if (pos_t nextPos = nowPos * _streambuf_expansion_magnification; nextPos <= target)
			__add_void_string(target - nowPos);
		else
		{
			__add_void_string(nextPos - nowPos);
			m_cdbStream.seekp(target);
		}
	}
}

void ConsoleDoubleBuffer::Clear()
{
	m_cdbStream.str("");
	m_cdbStream.clear();
}

void ConsoleDoubleBuffer::Flipping()
{
	if (m_isRun)
		return;

	static HANDLE std_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(std_output_handle, COORD{ 0, 0 });
	

	std::string cdbData = m_cdbStream.str();

	size_t dataLength = cdbData.size();
	if (dataLength < m_prevFlippedDataLength[0])
		cdbData.resize(m_prevFlippedDataLength[0], ' ');
	m_prevFlippedDataLength[0] = m_prevFlippedDataLength[1];
	m_prevFlippedDataLength[1] = dataLength;

	for (size_t i = m_widthLimite; i < cdbData.size(); i += (m_widthLimite + 1))
		cdbData.insert(i, 1, '\n');


	SetConsoleCursorPosition(m_back_buffer, COORD{ 0, 0 });
	WriteConsoleA(m_back_buffer, cdbData.data(), cdbData.size(), nullptr, nullptr);
	_buffer_swap();
}
