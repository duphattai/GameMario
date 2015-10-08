#pragma once
#include "stdafx.h"

class CKeyBoard
{
protected:
	LPDIRECTINPUT m_Input;
	LPDIRECTINPUTDEVICE m_Keyboard;

	int KeyCode;

	BYTE m_Keys[256];
	DIDEVICEOBJECTDATA m_KeyEvents[KEYBOARD_BUFFER_SIZE];
public:
	CKeyBoard();
	void initInput();
	void initKeyBoard(HWND);
	bool isKeyDown(int);
	int  getKeyCode();
	void clearKeyCode();
	bool isPressed(int);
	void getState();
	~CKeyBoard();
};

