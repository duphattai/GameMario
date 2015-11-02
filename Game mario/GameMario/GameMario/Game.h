#pragma once
#include "stdafx.h"

class Game
{
	HWND hWnd;
	HINSTANCE hInstance;
	LPDIRECT3DDEVICE9 d3ddv;
	LPDIRECT3D9 d3d;


	LPD3DXSPRITE m_SpriteHandler;
public:
	Game(HINSTANCE hInstance);
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void initGame();
	int intiWindow(HINSTANCE hInstance);
	void initDirec3D();

	void gameRun();
	void update();
	void render();
	void gameEnd();
	~Game();
};

