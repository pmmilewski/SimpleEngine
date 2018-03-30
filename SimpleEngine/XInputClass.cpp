#include "XInputClass.h"
#include <cmath>

XInputClass::XInputClass(float dzX, float dzY) : m_deadzoneX(dzX), m_deadzoneY(dzY), m_targetWindow(nullptr)
{
	SetButtons();
}

XInputClass::XInputClass() : m_deadzoneX(0.05f), m_deadzoneY(0.02f), m_targetWindow(nullptr)
{
	SetButtons();
}

XInputClass::~XInputClass()
= default;

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
		m_previousState = m_state;
		prevLeftStickY = leftStickY;
		prevLeftStickX = leftStickX;
		prevRightStickX = rightStickX;
		prevRightStickY = rightStickY;
		prevLeftTrigger = leftTrigger;
		prevRightTrigger = rightTrigger;
		ZeroMemory(&m_state, sizeof(XINPUT_STATE));
		if (XInputGetState(m_cId, &m_state) != ERROR_SUCCESS)
		{
			m_cId = -1;
			return false;
		}
		float normLX = fmaxf(-1, (float)m_state.Gamepad.sThumbLX / 32767);
		float normLY = fmaxf(-1, (float)m_state.Gamepad.sThumbLY / 32767);

		leftStickX = (abs(normLX) < m_deadzoneX ? 0 : (abs(normLX) - m_deadzoneX) * (normLX / abs(normLX)));
		leftStickY = (abs(normLY) < m_deadzoneY ? 0 : (abs(normLY) - m_deadzoneY) * (normLY / abs(normLY)));

		if (m_deadzoneX > 0) leftStickX *= 1 / (1 - m_deadzoneX);
		if (m_deadzoneY > 0) leftStickY *= 1 / (1 - m_deadzoneY);

		float normRX = fmaxf(-1, (float)m_state.Gamepad.sThumbRX / 32767);
		float normRY = fmaxf(-1, (float)m_state.Gamepad.sThumbRY / 32767);

		rightStickX = (abs(normRX) < m_deadzoneX ? 0 : (abs(normRX) - m_deadzoneX) * (normRX / abs(normRX)));
		rightStickY = (abs(normRY) < m_deadzoneY ? 0 : (abs(normRY) - m_deadzoneY) * (normRY / abs(normRY)));

		if (m_deadzoneX > 0) rightStickX *= 1 / (1 - m_deadzoneX);
		if (m_deadzoneY > 0) rightStickY *= 1 / (1 - m_deadzoneY);

		leftTrigger = (float)m_state.Gamepad.bLeftTrigger / 255;
		rightTrigger = (float)m_state.Gamepad.bRightTrigger / 255;

		if (m_targetWindow != nullptr)
		{
			for (const auto& button : m_Buttons)
			{
				if ((m_state.Gamepad.wButtons & button.first) != 0)
				{
					// Get key mapping or use XINPUT_GAMEPAD_* value if no mapping exists
					WORD mapping = (m_keyMap.find(button.first) != m_keyMap.end()) ?
						m_keyMap.find(button.first)->second : button.first;

					// Get current time and last WM_KEYDOWN message for repeat interval check
					DWORD now = GetTickCount();
					DWORD last = (m_lastPress.find(button.first) != m_lastPress.end() ?
						m_lastPress.find(button.first)->second : 0);

					// Find desired repeat interval for this button
					unsigned int ms = m_repeatMs.find(button.first)->second;

					// If first press, or repeat interval passed (and repeat interval != 0)
					if ((now - last >= ms && ms > 0)
						|| last == 0
						|| (ms == 0 && (m_previousState.Gamepad.wButtons & button.first) == 0))
					{
						// Update last press time
						m_lastPress.erase(button.first);
						m_lastPress.insert(std::map<WORD, DWORD>::value_type(button.first, now));

						SendMessage(m_targetWindow, WM_KEYDOWN, mapping,
							((m_previousState.Gamepad.wButtons & button.first) == 0 ? 0 << 30 : 1 << 30));
					}
				}

				// Checking for button release events, will cause the state
				// packet number to be incremented
				if (m_previousState.dwPacketNumber < m_state.dwPacketNumber)
				{
					// If the button was pressed but is no longer pressed
					if ((m_state.Gamepad.wButtons & button.first) == 0 && (m_previousState.Gamepad.wButtons & button.first) != 0)
					{
						// Get key mapping if one exists
						WORD mapping = (m_keyMap.find(button.first) != m_keyMap.end() ?
							m_keyMap.find(button.first)->second : button.first);

						// Remove last press time
						m_lastPress.erase(button.first);

						// Send keyboard event
						SendMessage(m_targetWindow, WM_KEYUP, mapping, 0);
					}
				}
			}

			for (auto item : m_analogMap)
			{
				WORD mapping = item.second.key;

				switch (item.first) {
				case AnalogButtons::LeftStickLeft:
					SendKeysOnTreshold(AnalogButtons::LeftStickLeft, leftStickX, prevLeftStickX, -item.second.threshold, mapping);
					break;

				case AnalogButtons::LeftStickRight:
					SendKeysOnTreshold(AnalogButtons::LeftStickRight, leftStickX, prevLeftStickX, item.second.threshold, mapping);
					break;

				case AnalogButtons::LeftStickUp:
					SendKeysOnTreshold(AnalogButtons::LeftStickUp, leftStickY, prevLeftStickY, item.second.threshold, mapping);
					break;

				case AnalogButtons::LeftStickDown:
					SendKeysOnTreshold(AnalogButtons::LeftStickDown, leftStickY, prevLeftStickY, -item.second.threshold, mapping);
					break;

				case AnalogButtons::RightStickLeft:
					SendKeysOnTreshold(AnalogButtons::RightStickLeft, rightStickX, prevRightStickX, -item.second.threshold, mapping);
					break;

				case AnalogButtons::RightStickRight:
					SendKeysOnTreshold(AnalogButtons::RightStickRight, rightStickX, prevRightStickX, item.second.threshold, mapping);
					break;

				case AnalogButtons::RightStickUp:
					SendKeysOnTreshold(AnalogButtons::RightStickUp, rightStickY, prevRightStickY, item.second.threshold, mapping);
					break;

				case AnalogButtons::RightStickDown:
					SendKeysOnTreshold(AnalogButtons::RightStickDown, rightStickY, prevRightStickY, -item.second.threshold, mapping);
					break;

				case AnalogButtons::LeftTrigger:
					SendKeysOnTreshold(AnalogButtons::LeftTrigger, leftTrigger, prevLeftTrigger, item.second.threshold, mapping);
					break;

				case AnalogButtons::RightTrigger:
					SendKeysOnTreshold(AnalogButtons::RightTrigger, rightTrigger, prevRightTrigger, item.second.threshold, mapping);
					break;
				}
			}
		}

		return true;
	}
	return false;
}

bool XInputClass::IsPressed(WORD button)
{

	return (m_state.Gamepad.wButtons & button) != 0;
}

void XInputClass::AddKeyMapping(WORD button, int key)
{
	m_keyMap.erase(button);
	m_keyMap.insert(std::map<WORD, int>::value_type(button, key));
}

void XInputClass::AddAnalogKeyMapping(AnalogButtons button, float threshold, int key)
{
	AnalogMapping a{ threshold, key };
	m_analogMap.erase(button);
	m_analogMap.insert(std::make_pair(button, a));
}

void XInputClass::RemoveKeyMappingByButton(WORD button)
{
	m_keyMap.erase(button);
}

void XInputClass::RemoveKeyMapping(int key)
{
	for (auto it = m_keyMap.begin(); it != m_keyMap.end(); ++it)
	{
		if (it->second == key)
		{
			m_keyMap.erase(it);
			break;
		}
	}
}

void XInputClass::RemoveAnalogKeyMapping(AnalogButtons button)
{
	m_analogMap.erase(button);
}

void XInputClass::ClearMappings()
{
	m_keyMap.clear();
	m_analogMap.clear();
}

void XInputClass::SetRepeatIntervalMsAll(unsigned int ms)
{
	m_repeatMs.clear();

	for (auto button : m_Buttons)
		m_repeatMs.insert(std::map<WORD, unsigned int>::value_type(button.first, ms));

	m_analogRepeatMs.clear();

	for (int i = 0; i < AnalogButtons::EndOfButtons; i++)
		m_analogRepeatMs.insert(std::map<AnalogButtons, unsigned int>::value_type((AnalogButtons)i, ms));
}

void XInputClass::SetRepeatIntervalMs(WORD button, unsigned int ms)
{
	m_repeatMs.erase(button);
	m_repeatMs.insert(std::map<WORD, unsigned int>::value_type(button, ms));
}

void XInputClass::SetAnalogRepeatIntervalMs(AnalogButtons button, unsigned int ms)
{
	m_analogRepeatMs.erase(button);
	m_analogRepeatMs.insert(std::map<AnalogButtons, unsigned int>::value_type(button, ms));
}

void XInputClass::SetWindow(HWND hwnd)
{
	m_targetWindow = hwnd;
}

void XInputClass::SetButtons()
{
	//m_keyMap.insert(std::map<WORD, int>::value_type(button, key));
	m_Buttons.insert(std::map<WORD, std::string>::value_type(XINPUT_GAMEPAD_A, "A"));
	m_Buttons.insert(std::map<WORD, std::string>::value_type(XINPUT_GAMEPAD_B, "B"));
	m_Buttons.insert(std::map<WORD, std::string>::value_type(XINPUT_GAMEPAD_X, "X"));
	m_Buttons.insert(std::map<WORD, std::string>::value_type(XINPUT_GAMEPAD_Y, "Y"));

	m_Buttons.insert(std::map<WORD, std::string>::value_type(XINPUT_GAMEPAD_DPAD_LEFT, "Left"));
	m_Buttons.insert(std::map<WORD, std::string>::value_type(XINPUT_GAMEPAD_DPAD_RIGHT, "Right"));
	m_Buttons.insert(std::map<WORD, std::string>::value_type(XINPUT_GAMEPAD_DPAD_UP, "Up"));
	m_Buttons.insert(std::map<WORD, std::string>::value_type(XINPUT_GAMEPAD_DPAD_DOWN, "Down"));

	m_Buttons.insert(std::map<WORD, std::string>::value_type(XINPUT_GAMEPAD_LEFT_SHOULDER, "LB"));
	m_Buttons.insert(std::map<WORD, std::string>::value_type(XINPUT_GAMEPAD_RIGHT_SHOULDER, "RB"));

	m_Buttons.insert(std::map<WORD, std::string>::value_type(XINPUT_GAMEPAD_BACK, "Back"));
	m_Buttons.insert(std::map<WORD, std::string>::value_type(XINPUT_GAMEPAD_START, "Start"));

	m_Buttons.insert(std::map<WORD, std::string>::value_type(XINPUT_GAMEPAD_LEFT_THUMB, "LS"));
	m_Buttons.insert(std::map<WORD, std::string>::value_type(XINPUT_GAMEPAD_RIGHT_THUMB, "RS"));

}

void XInputClass::SendKeysOnTreshold(AnalogButtons button, float now, float before, float threshold, int key)
{
	bool isPressed = (now >= threshold && threshold > 0) || (now <= threshold && threshold < 0);
	bool wasPressed = (before >= threshold && threshold > 0) || (before <= threshold && threshold < 0);

	if (isPressed)
	{
		// Repeat interval calculation
		DWORD now = GetTickCount();
		DWORD last = (m_analogLastPress.find(button) != m_analogLastPress.end() ?
			m_analogLastPress.find(button)->second : 0);

		unsigned int ms = m_analogRepeatMs.find(button)->second;

		if ((now - last >= ms && ms > 0) || last == 0 || (ms == 0 && !wasPressed))
		{
			m_analogLastPress.erase(button);
			m_analogLastPress.insert(std::map<AnalogButtons, DWORD>::value_type(button, now));
			SendMessage(m_targetWindow, WM_KEYDOWN, key, (wasPressed ? 1 << 30 : 0 << 30));
		}
	}

	if (m_previousState.dwPacketNumber < m_state.dwPacketNumber)
	{
		if (!isPressed && wasPressed)
		{
			m_analogLastPress.erase(button);
			SendMessage(m_targetWindow, WM_KEYUP, key, 0);
		}
	}
}
