#include "ConsoleBuffer.h"

ConsoleBuffer::ConsoleBuffer()
	: m_buffer{ nullptr, nullptr }
	, m_size{ NULL, NULL }
	, m_bid(NULL)
	, m_cursorPos{ NULL, NULL }
{
}
ConsoleBuffer::~ConsoleBuffer()
{
}


void ConsoleBuffer::Initialize(short sizeX, short sizeY)
{
	m_size.x = sizeX;
	m_size.y = sizeX;

	COORD size = { m_size.x, m_size.y }; //버퍼의 크기 관련 구조채
	SMALL_RECT rect; //창 크기 관련 구조체
	rect.Left = 0;
	rect.Top = 0;
	rect.Right = m_size.x - 1;
	rect.Bottom = m_size.y - 1;
	//버퍼를 생성하기 전에 버퍼의 크기와 창의 크기를 설정할 구조체 COORD와 SMALL_RECT를 선언해주고 원하는 크기를 위와 같이 선언하시면 됩니다.

	m_buffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL); //버퍼 생성
	m_buffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL); //버퍼 생성
	SetConsoleScreenBufferSize(m_buffer[0], size); //콘솔 버퍼의 크기 설정
	SetConsoleScreenBufferSize(m_buffer[1], size); //콘솔 버퍼의 크기 설정
	SetConsoleWindowInfo(m_buffer[0], TRUE, &rect); //콘솔 창의 크기 설정
	SetConsoleWindowInfo(m_buffer[1], TRUE, &rect); //콘솔 창의 크기 설정
	
	m_bid = 0;
	SetConsoleActiveScreenBuffer(m_buffer[_showing()]);
}

void ConsoleBuffer::Release()
{
	CloseHandle(m_buffer[0]);
	CloseHandle(m_buffer[1]);
	m_buffer[0] = nullptr;
	m_buffer[1] = nullptr;
}


void ConsoleBuffer::Flipping()
{
	m_bid = m_bid ? 0 : 1;
	SetConsoleActiveScreenBuffer(m_buffer[_showing()]);
}

void ConsoleBuffer::Clear()
{
	COORD Coor = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(m_buffer[_drawing()], ' ', m_size.x * m_size.y, Coor, &dw);
}


void ConsoleBuffer::CursorMove(short x, short y) { CursorMove(COORD{ x, y }); }
void ConsoleBuffer::CursorMove(COORD pos)
{
	DWORD dw;
	m_cursorPos = pos;
	SetConsoleCursorPosition(m_buffer[_drawing()], m_cursorPos);    //좌표 이동
}

void ConsoleBuffer::OutputText(std::string_view text, COORD pos)
{
	DWORD dw;
	WriteConsoleOutputCharacter(m_buffer[_drawing()], text.data(), text.size(), pos, &dw);
	m_cursorPos.X += dw;
}
