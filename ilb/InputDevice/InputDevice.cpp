#include "InputDevice.h"
#include <windowsx.h>

InputDevice::InputDevice()
	: m_isWndActivate(true)

	, m_mousePos{ 0, 0 }
	, m_mouseDelta{ 0, 0 }
	, m_wheelDelta(0)
{
	m_keyState.fill(KeyState::None);
}
InputDevice::~InputDevice()
{
}



void InputDevice::BeginFrame(HWND& hWnd)
{
	POINT prevPos = m_mousePos;
	GetCursorPos(&m_mousePos);
	ScreenToClient(hWnd, &m_mousePos);
	m_mouseDelta.x = m_mousePos.x - prevPos.x;
	m_mouseDelta.y = m_mousePos.y - prevPos.y;

	for (size_t index = 0; index < c_keyMax; index++)
	{
		KeyState& keyPlace = m_keyState[index];
		keyPlace = m_isWndActivate && GetAsyncKeyState(index) ?
			(keyPlace == KeyState::Pressed	|| keyPlace == KeyState::Down	? KeyState::Pressed : KeyState::Down) :
			(keyPlace == KeyState::None		|| keyPlace == KeyState::Up		? KeyState::None	: KeyState::Up);
	}
}

void InputDevice::EndFrame()
{
	m_wheelDelta = 0;
}

void InputDevice::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_ACTIVATE:
		m_isWndActivate = wParam == 1 || wParam == 2;
		break;

	case WM_MOUSEWHEEL:
		m_wheelDelta += GET_WHEEL_DELTA_WPARAM(wParam);
		break;
	}
}
