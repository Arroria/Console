#pragma once
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <string>
#include <array>

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
	static constexpr Color_t _default_color = Color_t::Gray;

	ColoredDoubleBufferConsole();
	~ColoredDoubleBufferConsole();

	bool Initialize();
	bool Initialize(short x, short y);
	void Release();

	template <typename data_t>
	ColoredDoubleBufferConsole& operator<<(const data_t& data);

	void SetColor(Color_t color) { return SetColor(size_t(color)); }
	void SetColor(size_t color);
	void SetCursorPos(short x, short y) { return SetCursorPos(COORD{ x, y }); }
	void SetCursorPos(COORD pos);

	void Clear();
	void Flipping();


private:
	// Buffer
	const HANDLE m_stdOutputHandle;
	std::array<std::stringstream, Color_t::__MAX> m_buffer;
	std::stringstream* m_activatedBuffer;

	COORD m_bufferCoordSize;
	size_t m_prevFlippedBufferLength;




	void SetCursorPos(std::streampos pos);
	
	size_t _get_buffer_size(size_t bufferColor) { return _get_buffer_size(Color_t(bufferColor)); }
	size_t _get_buffer_size(Color_t bufferColor) { return _get_buffer_size(m_buffer[bufferColor]); }
	size_t _get_buffer_size(std::stringstream& buffer)
	{
		auto p = buffer.tellp();
		buffer.seekp(0, std::ios::end);
		auto result = buffer.tellp();
		buffer.seekp(p);
		return result;
	}
};



template<typename data_t>
inline ColoredDoubleBufferConsole& ColoredDoubleBufferConsole::operator<<(const data_t& data)
{
	std::streampos start = m_activatedBuffer->tellp();
	(*m_activatedBuffer) << data;
	std::streampos end = m_activatedBuffer->tellp();

	bool run = false;
	for (size_t i = 0; i < m_buffer.size(); i++)
	{
		if (!run)
		{
			if (std::addressof(m_buffer[i]) == m_activatedBuffer)
				run = true;
		}
		else
		{
			m_buffer[i].seekp(0, std::ios::end);
			if (m_buffer[i].tellp() < start)
				continue;
		
			m_buffer[i].seekp(start);
			m_buffer[i] << std::string(end - start, ' ');
		}
	}
	return *this;
}

