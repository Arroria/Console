#include "TetrisGame.h"

TetrisGame::TetrisGame(size_t mapSizeX, size_t mapSizeY)
	: m_tetris(mapSizeX, mapSizeY)

	, m_prevUpdateTime(_Clock_t::now())
	, m_blockAutoDropCooldown(_blockAutoDropCooldownMax)
{
}

TetrisGame::~TetrisGame()
{
}



#include "../ilb/InputDevice/InputDevice.h"
#pragma comment (lib, "../ilb/InputDevice/InputDeviceD.lib")
void TetrisGame::Update()
{
	static InputDevice inputDevice;

	char pszOldWindowTitle[1024];
	GetConsoleTitleA(pszOldWindowTitle, 1024);
	HWND hwnd = FindWindowA(NULL, pszOldWindowTitle);
	inputDevice.BeginFrame(hwnd);


	auto nowTime = _Clock_t::now();
	_Time_t deltaTime = nowTime - m_prevUpdateTime;

	m_blockAutoDropCooldown -= deltaTime;

	if (inputDevice.IsKeyDown('Q'))		m_tetris.ControllerRotationL();
	if (inputDevice.IsKeyDown('E'))		m_tetris.ControllerRotationR();
	if (inputDevice.IsKeyDown(VK_LEFT))	m_tetris.ControllerMoveL();
	if (inputDevice.IsKeyDown(VK_RIGHT))	m_tetris.ControllerMoveR();
	if (bool autoDown = m_blockAutoDropCooldown <= _Time_t::zero(); inputDevice.IsKeyDown(VK_DOWN) || autoDown)
	{
		if (!m_tetris.ControllerDown() && autoDown)
			m_tetris.ControllerEmplace();
		m_blockAutoDropCooldown = _blockAutoDropCooldownMax;
	}
	if (inputDevice.IsKeyDown(VK_SPACE))
	{
		while (m_tetris.ControllerDown());
		m_tetris.ControllerEmplace();
		m_blockAutoDropCooldown = _blockAutoDropCooldownMax;
	}

	m_prevUpdateTime = nowTime;
	inputDevice.EndFrame();
}

void TetrisGame::Render(ConsoleDoubleBuffer& cdb)
{
	m_tetris.Render(cdb);
}
