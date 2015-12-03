#pragma once
#include "stdafx.h"

class CKeyBoard
{
protected:
	LPDIRECTINPUT m_Input;
	LPDIRECTINPUTDEVICE m_Keyboard;

	BYTE		m_PreviousKeys[256];
	int			KeyCode;

	BYTE m_Keys[256];

	bool initInput();
	bool initKeyBoard(HWND);
public:
	CKeyBoard();
	bool initialize(HWND);
	bool isKeyDown(int);
	int  getKeyCode();
	bool isPressed(int);
	void getState();
	~CKeyBoard();
};

