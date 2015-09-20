#include "Game.h"

#include "KeyBoard.h"
#include "Source.h"

CKeyBoard *keyboard = NULL;
Game::Game(HINSTANCE hInstance)
{
	this->hInstance = hInstance;
}

LRESULT Game::WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		//Source::GetInstance()->Destroy();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int Game::intiWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Game";
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	hWnd = CreateWindow(
		"Game",
		MAIN_WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW,
		//			WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd) {
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, SW_NORMAL);
	UpdateWindow(hWnd);

	return 1;
}

void Game::initGame()//khởi tạo 
{
	//Source::getInstance()->Init(d3ddv);

	//keyboard = new CKeyBoard();//process event of keyboard
	//keyboard->InitInput();
	//keyboard->InitKeyBoard(hWnd);

	//m_state = new Map1();
	//m_state->Init(keyboard);
}

void Game::initDirec3D()//initialize Dir3D
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	HRESULT result;
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	result = d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddv);

	if (result != D3D_OK)
	{
		MessageBox(0, "Cannot create device", "Error", MB_OK);
		return;
	}

	D3DXCreateSprite(d3ddv, &m_SpriteHandler);
}

void Game::render()
{
	d3ddv->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, NULL);

	d3ddv->BeginScene();
	m_SpriteHandler->Begin(D3DXSPRITE_ALPHABLEND);//begin of the draw sprite

	//m_state->Draw(m_SpriteHandler);

	m_SpriteHandler->End();//end action draw sprite
	d3ddv->EndScene();

	d3ddv->Present(NULL, NULL, NULL, NULL);//draw from buffer to screen
}

void Game::update()
{
	//m_state->Update();
}

void Game::gameRun()
{
	update();
	render();
}
Game::~Game()
{
	if (m_SpriteHandler != NULL)
		m_SpriteHandler->Release();

	if (d3ddv != NULL)
		d3ddv->Release();

	if (d3d != NULL)
		d3d->Release();
}
