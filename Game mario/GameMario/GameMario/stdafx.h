#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include <dinput.h>
#include <windows.h>

// playsound 
#pragma comment(lib, "winmm.lib")

// -----------------------windowed
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 240
#define MAIN_WINDOW_TITLE L"Sample"
#define FPS 30

// -------------------character
#define GROUND_Y 200
#define VIEW_PORT_Y 240
#define GRAVITATION	-1

// xet id phu thuoc vao file text quy dinh
// 0.* = di chuyen tren mat dat  



//-----------------------------key board
#pragma comment(lib, "dinput8.lib")
#pragma comment (lib,"dxguid.lib") 
#define KEYBOARD_BUFFER_SIZE 1024

// ----------------Data
#define TileMapPath L"Map//tileset.png"
#define NodeTileMap_1 "Map//1-1//TileNode.xml"


// --------------------Bullet
#define PI 3.14159f
#define TimeLiveSeconds 1.0f
#define SpeedBullet 16.5f


//------ Item
#define ItemTimeAnimation 0.2f
