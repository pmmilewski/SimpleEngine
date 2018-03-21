////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	void LMBClicked(int, int);
	void LMBReleased(int, int);

	void UpdateMouseX(int);
	void UpdateMouseY(int);

	bool IsKeyDown(unsigned int);

private:
	bool m_keys[256];
	bool m_lmbStatus;
	int m_mouseX, m_mouseY;
	int m_mouseXClicked, m_mouseYClicked;
	int m_mouseXReleased, m_mouseYReleased;

};

#endif