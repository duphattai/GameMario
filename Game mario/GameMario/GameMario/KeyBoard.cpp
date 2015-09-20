#include "KeyBoard.h"


CKeyBoard::CKeyBoard()
{
	KeyCode = -1;
}

void CKeyBoard::initInput()
{
	HRESULT result;

	result = DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_Input,
		NULL);

	if (result != D3D_OK)
	{
		MessageBox(0, "Cannot create device input.", "Error", MB_OK);
		return;
	}

	//Create device use

	result = m_Input->CreateDevice(GUID_SysKeyboard, &m_Keyboard, NULL);

	if (result != D3D_OK)
	{
		MessageBox(0, "Cannot create device keyboard.", "Error", MB_OK);
		return;
	}
}

void CKeyBoard::initKeyBoard(HWND hWnd)
{
	// Directx understand handler with keyboard device
	m_Keyboard->SetDataFormat(&c_dfDIKeyboard);

	// thiet lap quyen uu tien
	m_Keyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE);

	DIPROPDWORD dipw;

	dipw.diph.dwSize = sizeof(DIPROPDWORD);
	dipw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipw.dwData = 256;
	dipw.diph.dwHow = DIPH_DEVICE;
	dipw.diph.dwObj = 0;

	m_Keyboard->SetProperty(DIPROP_BUFFERSIZE, &dipw.diph);

	m_Keyboard->Acquire();
}

bool CKeyBoard::isKeyDown(int key)
{
	if ((m_Keys[key] & 0x80) != 0)
	{
		KeyCode = key;
		return true;
	}
	return false;
}

int CKeyBoard::getKeyCode()
{
	return KeyCode;
}

void CKeyBoard::clearKeyCode()
{
	KeyCode = -1;
}

void CKeyBoard::getState()
{
	m_Keyboard->GetDeviceState(sizeof(m_Keys), (LPVOID)&m_Keys);

	DWORD dwElements = KEYBOARD_BUFFER_SIZE;
	m_Keyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_KeyEvents, &dwElements, 0);
}

bool CKeyBoard::isPressed(int Key)
{
	for (DWORD i = 0; i < KEYBOARD_BUFFER_SIZE; i++)
	{
		int KeyCode = m_KeyEvents[i].dwOfs;
		int KeyState = m_KeyEvents[i].dwData;

		if (KeyCode == Key)
		{
			if ((KeyState & 0x80) != 0)
				return true;
			else
				return false;
		}
	}
}

CKeyBoard::~CKeyBoard()
{
}
