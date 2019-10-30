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
	_primitive_text_color(_default_color);
	m_bufferCoordSize = COORD{ x, y };
	return true;
}

void ColoredDoubleBufferConsole::Release()
{
	_primitive_text_color(_default_color);
}



void ColoredDoubleBufferConsole::SetColor(size_t color)
{
	std::streampos prevPos = m_activatedBuffer->tellp();
	m_activatedBuffer = std::addressof(m_buffer[color]);
	SetCursorPos(prevPos);
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
			str.resize(pos - tellp, (m_activatedBuffer == std::addressof(m_buffer[_default_color]) ? ' ' : _draw_ignore_code));
			(*m_activatedBuffer) << str;
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
	constexpr auto string_find_void = [](std::string& str, size_t pos)->size_t { return str.find(_draw_ignore_code, pos); };
	constexpr auto string_find_anything = [](std::string& str, size_t pos)->size_t
		{
			while (pos < str.size())
			{
				if (str[pos] != _draw_ignore_code)
					return pos;
				++pos;
			}
			return std::string::npos;
		};
	constexpr auto string_available_length = [](std::string& str)->size_t
	{
		for (int pos = str.size() - 1; 0 <= pos; --pos)
		{
			if (str[pos] != _draw_ignore_code)
				return pos;
		}
		return std::string::npos;
	};
	
	
	std::streampos _prev_pos = m_activatedBuffer->tellp();

	_primitive_cursor_move(0);
	_primitive_text_color(_default_color);
	std::cout << m_buffer[_default_color].str();

	size_t maxBufferSize = _get_buffer_size_unsafe(m_buffer[_default_color]);

	for (size_t index = 0; index < m_buffer.size(); ++index)
	{
		if (index == size_t(_default_color))
			continue;
		
		auto& indexBuffer = m_buffer[index];
		std::string bufferStr(std::move(indexBuffer.str()));
		
		size_t nowBufferSize = string_available_length(bufferStr);
		if (nowBufferSize == std::string::npos)
			continue;
		if (maxBufferSize < nowBufferSize)
			maxBufferSize = nowBufferSize;

		size_t start(0), end(0);
		_primitive_text_color(index);
		while (true)
		{
			start = string_find_anything(bufferStr, end);
			end = string_find_void(bufferStr, start);
			if (start == std::string::npos)
				break;

			_primitive_cursor_move(start);
			std::cout << bufferStr.substr(start, end - start);
		}
	}

	if (m_prevFlippedBufferLength > maxBufferSize)
	{
		_primitive_cursor_move(maxBufferSize);
		_primitive_text_color(_default_color);
		std::cout << std::string(m_prevFlippedBufferLength - maxBufferSize, ' ');
	}
	m_prevFlippedBufferLength = maxBufferSize;

	m_activatedBuffer->seekp(_prev_pos);
}
