#pragma once
#include <chrono>
#include "Tetris.h"

class TetrisGame
{
public:
	using _Clock_t = std::chrono::steady_clock;
	using _Time_t = _Clock_t::duration;

	static constexpr _Time_t _blockAutoDropCooldownMax = std::chrono::milliseconds(1000);

	TetrisGame(size_t mapSizeX, size_t mapSizeY);
	~TetrisGame();


	void Update();
	void Render(ConsoleDoubleBuffer& cdb);

private:
	Tetris m_tetris;

	_Clock_t::time_point m_prevUpdateTime;
	_Time_t m_blockAutoDropCooldown;
};

