#include "ColoredDoubleBufferConsole.h"

ColoredDoubleBufferConsole::ColoredDoubleBufferConsole()
	: m_stdOutputHandle(GetStdHandle(STD_OUTPUT_HANDLE))
	, m_buffer()
	, m_activatedBuffer(std::addressof(m_buffer[size_t(_default_color)]))

	, m_bufferCoordSize{ NULL, NULL }
	, m_prevFlippedBufferLength(0)
{
}

ColoredDoubleBufferConsole::~ColoredDoubleBufferConsole()
{
}



bool ColoredDoubleBufferConsole::Initialize()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(m_stdOutputHandle, &csbi))
		return false;

	return Initialize(csbi.dwSize.X, csbi.dwSize.Y);
}

bool ColoredDoubleBufferConsole::Initialize(short x, short y)
{
	m_bufferCoordSize = COORD{ x, y };
	return true;
}

void ColoredDoubleBufferConsole::Release()
{
	SetConsoleTextAttribute(m_stdOutputHandle, size_t(_default_color));
}



void ColoredDoubleBufferConsole::SetColor(size_t color)
{
	std::streampos prevPos = m_activatedBuffer->tellp();
	m_activatedBuffer = std::addressof(m_buffer[color]);
	SetCursorPos(prevPos);
}

void ColoredDoubleBufferConsole::SetCursorPos(COORD pos)
{
	return SetCursorPos(size_t(pos.X) + size_t(pos.Y) * size_t(m_bufferCoordSize.X));
}

void ColoredDoubleBufferConsole::SetCursorPos(std::streampos pos)
{
	if (m_activatedBuffer->seekp(pos).fail())
	{
		m_activatedBuffer->clear();
		m_activatedBuffer->seekp(0, std::ios::end);
		if (auto tellp = m_activatedBuffer->tellp(); tellp < pos)
		{
			std::string str;
			str.resize(pos - tellp, ' ');
			(*this) << str;
		}
	}
}


void ColoredDoubleBufferConsole::Clear()
{
	for (auto iter = m_buffer.begin(); iter != m_buffer.end(); ++iter)
	{
		iter->str("");
		iter->clear();
	}
}

void ColoredDoubleBufferConsole::Flipping()
{
	SetCursorPos(0, 0);
	SetConsoleCursorPosition(m_stdOutputHandle, COORD{ 0, 0 });
	SetConsoleTextAttribute(m_stdOutputHandle, size_t(_default_color));
	std::cout << m_buffer[size_t(_default_color)].str();
	size_t maxBufferSize = _get_buffer_size(_default_color);

	for (size_t index = 0; index < m_buffer.size(); ++index)
	{
		constexpr auto string_find_void = [](std::string& str, size_t pos)->size_t { return str.find(' ', pos); };
		constexpr auto string_find_anything = [](std::string& str, size_t pos)->size_t
		{
			while (pos < str.size())
			{
				if (str[pos] != ' ')
					return pos;
				++pos;
			}
			return std::string::npos;
		};
		auto& nowBuffer = m_buffer[index];
		
		if (index == size_t(_default_color))
			continue;


		size_t nowBufferSize = _get_buffer_size(nowBuffer);
		if (!nowBufferSize)
			continue;
		if (maxBufferSize < nowBufferSize)
			maxBufferSize = nowBufferSize;

		SetConsoleTextAttribute(m_stdOutputHandle, index);
		std::string bufferStr(std::move(nowBuffer.str()));
		size_t start(0), end(0);
		while (true)
		{
			start = string_find_anything(bufferStr, end);
			end = string_find_void(bufferStr, start);
			if (start == std::string::npos)
				break;

			SetConsoleCursorPosition(m_stdOutputHandle, COORD{ short(start), 0 });    //ÁÂÇ¥ ÀÌµ¿
			std::cout << bufferStr.substr(start, end - start);
		}
	}

	if (m_prevFlippedBufferLength > maxBufferSize)
	{
		SetConsoleCursorPosition(m_stdOutputHandle, COORD{ short(maxBufferSize), 0 });
		std::cout << std::string(m_prevFlippedBufferLength - maxBufferSize, ' ');
	}
	m_prevFlippedBufferLength = maxBufferSize;
}
