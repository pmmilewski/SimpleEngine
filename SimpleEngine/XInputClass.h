#pragma once
#ifndef _XINPUTCLASS_H_
#define _XINPUTCLASS_H_

// Includes
#include <Xinput.h>
#include <windows.h>

class XInputClass
{
private:
	int m_cId;
	XINPUT_STATE m_state;

	float deadzoneX, deadzoneY;

public:
	XInputClass();
	XInputClass(float dzX, float dzY);
	~XInputClass();

	float leftStickX, leftStickY;
	float rightStickX, rightStickY;
	float leftTrigger, rightTrigger;

	int GetPort();
	XINPUT_GAMEPAD* GetState();
	bool CheckConnection();
	bool Refresh();
	bool IsPressed(WORD);
};

#endif