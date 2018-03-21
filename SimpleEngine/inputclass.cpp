////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "inputclass.h"


InputClass::InputClass()
{
}


InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass()
{
}


void InputClass::Initialize()
{
	int i;
	

	// Initialize all the keys to being released and not pressed.
	for(i=0; i<256; i++)
	{
		m_keys[i] = false;
	}

	m_lmbStatus = false;
	m_mouseX = 0;
	m_mouseY = 0;
	m_mouseXClicked = 0;
	m_mouseYClicked = 0;
	m_mouseXReleased = 0;
	m_mouseYReleased = 0;

	return;
}


void InputClass::KeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	m_keys[input] = true;
	return;
}


void InputClass::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	m_keys[input] = false;
	return;
}


void InputClass::LMBClicked(int mouseX, int mouseY)
{
	m_lmbStatus = true;
	m_mouseXClicked = mouseX;
	m_mouseYClicked = mouseY;
}

void InputClass::LMBReleased(int mouseX, int mouseY)
{
	m_lmbStatus = false;
	m_mouseXReleased = mouseX;
	m_mouseYReleased = mouseY;
}

void InputClass::UpdateMouseX(int mouseX)
{
	m_mouseX = mouseX;
}

void InputClass::UpdateMouseY(int mouseY)
{
	m_mouseY = mouseY;
}


bool InputClass::IsKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return m_keys[key];
}