#pragma once
#ifndef _XINPUTCLASS_H_
#define _XINPUTCLASS_H_

// Includes
#include <windows.h>
#include <Xinput.h>
#include <map>

class XInputClass
{
private:
	enum AnalogButtons {
		LeftStickLeft,
		LeftStickRight,
		LeftStickUp,
		LeftStickDown,
		RightStickLeft,
		RightStickRight,
		RightStickUp,
		RightStickDown,
		LeftTrigger,
		RightTrigger,
		EndOfButtons
	};

	struct AnalogMapping {
		float threshold;
		int key;
	};

private:
	int m_cId;
	XINPUT_STATE m_state;
	XINPUT_STATE m_previousState;
	float m_deadzoneX, m_deadzoneY;
	HWND m_targetWindow;

	std::map<WORD, int> m_keyMap;
	std::map<AnalogButtons, AnalogMapping> m_analogMap;

	std::map<WORD, unsigned int> m_repeatMs;
	std::map<AnalogButtons, unsigned int> m_analogRepeatMs;

	std::map<WORD, DWORD> m_lastPress;
	std::map<AnalogButtons, DWORD> m_analogLastPress;

	void SetButtons();
	void SendKeysOnTreshold(AnalogButtons, float, float, float, int);

public:
	XInputClass();
	XInputClass(float dzX, float dzY);
	~XInputClass();

	std::map<WORD, std::string> m_Buttons;

	float leftStickX, leftStickY;
	float prevLeftStickX, prevLeftStickY;
	float rightStickX, rightStickY;
	float prevRightStickX, prevRightStickY;
	float leftTrigger, rightTrigger;
	float prevLeftTrigger, prevRightTrigger;

	int GetPort();
	XINPUT_GAMEPAD* GetState();
	bool CheckConnection();
	bool Refresh();
	bool IsPressed(WORD);

	void AddKeyMapping(WORD, int);
	void AddAnalogKeyMapping(AnalogButtons, float, int);
	void RemoveKeyMappingByButton(WORD);
	void RemoveKeyMapping(int);
	void RemoveAnalogKeyMapping(AnalogButtons);
	void ClearMappings();

	void SetRepeatIntervalMsAll(unsigned int);
	void SetRepeatIntervalMs(WORD, unsigned int);
	void SetAnalogRepeatIntervalMs(AnalogButtons, unsigned int);

	void SetWindow(HWND);
};

#endif