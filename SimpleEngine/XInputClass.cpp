#include "XInputClass.h"
#include <cmath>

XInputClass::XInputClass(float dzX, float dzY) : deadzoneX(dzX), deadzoneY(dzY)
{

}

XInputClass::XInputClass() : deadzoneX(0.05f), deadzoneY(0.02f)
{

}

XInputClass::~XInputClass()
{
}

int XInputClass::GetPort()
{
	return m_cId + 1;
}

XINPUT_GAMEPAD* XInputClass::GetState()
{
	return &m_state.Gamepad;
}

bool XInputClass::CheckConnection()
{
	int controllerId = -1;

	for (DWORD i = 0; i < XUSER_MAX_COUNT && controllerId == -1; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		if (XInputGetState(i, &state) == ERROR_SUCCESS)
			controllerId = i;
	}

	m_cId = controllerId;

	return controllerId != -1;
}

bool XInputClass::Refresh()
{
	if (m_cId == -1)
		CheckConnection();

	if (m_cId != -1)
	{
		ZeroMemory(&m_state, sizeof(XINPUT_STATE));
		if (XInputGetState(m_cId, &m_state) != ERROR_SUCCESS)
		{
			m_cId = -1;
			return false;
		}
		float normLX = fmaxf(-1, (float)m_state.Gamepad.sThumbLX / 32767);
		float normLY = fmaxf(-1, (float)m_state.Gamepad.sThumbLY / 32767);

		leftStickX = (abs(normLX) < deadzoneX ? 0 : (abs(normLX) - deadzoneX) * (normLX / abs(normLX)));
		leftStickY = (abs(normLY) < deadzoneY ? 0 : (abs(normLY) - deadzoneY) * (normLY / abs(normLY)));

		if (deadzoneX > 0) leftStickX *= 1 / (1 - deadzoneX);
		if (deadzoneY > 0) leftStickY *= 1 / (1 - deadzoneY);

		float normRX = fmaxf(-1, (float)m_state.Gamepad.sThumbRX / 32767);
		float normRY = fmaxf(-1, (float)m_state.Gamepad.sThumbRY / 32767);

		rightStickX = (abs(normRX) < deadzoneX ? 0 : (abs(normRX) - deadzoneX) * (normRX / abs(normRX)));
		rightStickY = (abs(normRY) < deadzoneY ? 0 : (abs(normRY) - deadzoneY) * (normRY / abs(normRY)));

		if (deadzoneX > 0) rightStickX *= 1 / (1 - deadzoneX);
		if (deadzoneY > 0) rightStickY *= 1 / (1 - deadzoneY);

		leftTrigger = (float)m_state.Gamepad.bLeftTrigger / 255;
		rightTrigger = (float)m_state.Gamepad.bRightTrigger / 255;

		return true;
	}
	return false;
}

bool XInputClass::IsPressed(WORD button)
{

	return (m_state.Gamepad.wButtons & button) != 0;
}
