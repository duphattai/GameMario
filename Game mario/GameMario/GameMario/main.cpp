#include "Game.h"
#include "KeyBoard.h"
#include "stdafx.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Game *game = new Game(hInstance);
	game->intiWindow(hInstance);
	game->initDirec3D();

	game->initGame();
	MSG msg;

	DWORD lasttime = GetTickCount();
	while (true)
	{
		DWORD currentime = GetTickCount();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		if (currentime - lasttime > (float)1000 / FPS)
		{
			lasttime = currentime;
			game->gameRun();
		}
	}

	delete game;
	return 1;
}