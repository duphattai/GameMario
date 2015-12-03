#include "Game.h"
#include "KeyBoard.h"
#include "stdafx.h"
#include "GameTime.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Game *game = new Game(hInstance);
	game->intiWindow(hInstance);
	game->initDirec3D();

	game->initGame();
	MSG msg;

	GameTime::getInstance()->setTime(); // set time for lasttime
	while (true)
	{
		GameTime::getInstance()->setTime(); // set time for current time
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

		if (GameTime::getInstance()->getElapsedTime() > (float)1000 / FPS)
		{
			GameTime::getInstance()->update();
			game->gameRun();
		}
	}

	delete game;
	return 1;
}