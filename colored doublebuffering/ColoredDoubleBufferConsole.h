#pragma once
#define NOMINMAX
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <string>
#include <array>
#include <type_traits>

class ColoredDoubleBufferConsole
{
public:
	enum Color_t : size_t
	{
		Black,
		DarkBlue,
		DarkGreen,
		DarkCyan,
		DarkRed,
		DarkMagenta,
		DarkYellow,
		Gray,
		DarkGray,
		Blue,
		Green,
		Cyan,
		Red,
		Magenta,
		Yellow,
		White,
		__MAX
	};
	static constexpr char _draw_ignore_code = 127;

	ColoredDoubleBufferConsole(Color_t startColor = Color_t::Gray);
	~ColoredDoubleBufferConsole();

	bool Initialize();
	bool Initialize(short x, short y);

	template <typename data_t>
	ColoredDoubleBufferConsole& operator<<(const data_t& data);

	void SetColor(Color_t color) { return SetColor(size_t(color)); }
	void SetColor(size_t color);
	void SetCursorPos(short x, short y) { SetCursorPos(x + y * m_bufferCoordSize.X); }

	void Clear();
	void Flipping();


private:
	// Buffer
	const HANDLE m_stdOutputHandle;
	std::array<std::stringstream, Color_t::__MAX> m_buffer;
	std::stringstream* m_activatedBuffer;

	COORD m_bufferCoordSize;



	void SetCursorPos(std::streampos pos);
	
	size_t _get_buffer_size(size_t bufferColor) { return _get_buffer_size(Color_t(bufferColor)); }
	size_t _get_buffer_size(Color_t bufferColor) { return _get_buffer_size(m_buffer[bufferColor]); }
	size_t _get_buffer_size(std::stringstream& buffer)
	{
		auto p = buffer.tellp();
		auto result = _get_buffer_size_unsafe(buffer);
		buffer.seekp(p);
		return result;
	}

	size_t _get_buffer_size_unsafe(size_t bufferColor) { return _get_buffer_size_unsafe(Color_t(bufferColor)); }
	size_t _get_buffer_size_unsafe(Color_t bufferColor) { return _get_buffer_size_unsafe(m_buffer[bufferColor]); }
	size_t _get_buffer_size_unsafe(std::stringstream& buffer)
	{
		buffer.seekp(0, std::ios::end);
		return buffer.tellp();
	}


	void _primitive_cursor_move(std::streampos spos)
	{
		short y = int(spos) / m_bufferCoordSize.X;
		short x = int(spos) - y * m_bufferCoordSize.X;
		_primitive_cursor_move(COORD{ x, y });
	}
	void _primitive_cursor_move(short x, short y)	{ _primitive_cursor_move(COORD{ x, y }); }
	void _primitive_cursor_move(COORD pos)			{ SetConsoleCursorPosition(m_stdOutputHandle, pos); }
	void _primitive_text_color(Color_t color)		{ _primitive_text_color(size_t(color)); }
	void _primitive_text_color(size_t colorCode)	{ SetConsoleTextAttribute(m_stdOutputHandle, colorCode); }

};



template<typename data_t>
inline ColoredDoubleBufferConsole& ColoredDoubleBufferConsole::operator<<(const data_t& data)
{
	size_t drawBegin = m_activatedBuffer->tellp();
	(*m_activatedBuffer) << data;
	size_t drawEnd = m_activatedBuffer->tellp();

	for (size_t index = m_activatedBuffer - std::addressof(m_buffer[0]) + 1; index < m_buffer.size(); ++index)
	{
		auto& buffer = m_buffer[index];

		buffer.seekp(0, std::ios::end);
		size_t bufferEnd = buffer.tellp();
		if (!bufferEnd || bufferEnd < drawBegin)
			continue;

		buffer.seekp(drawBegin);
		buffer << std::string(std::min(drawEnd - drawBegin, bufferEnd - drawBegin), _draw_ignore_code);
	}
	return *this;
}

