#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include <dinput.h>
#include "Global.h"

// -----------------------windowed
#define SCALE 1
#define SCREEN_WIDTH 256 * SCALE
#define SCREEN_HEIGHT 240 * SCALE
#define MAIN_WINDOW_TITLE L"Sample"
#define FPS 30

// -------------------character
#define GROUND_Y 200
#define VIEW_PORT_Y 240* SCALE
#define GRAVITATION	-1

// xet id phu thuoc vao file text quy dinh
// 0.* = di chuyen tren mat dat  



//-----------------------------key board
#pragma comment(lib, "dinput8.lib")
#pragma comment (lib,"dxguid.lib") 
#define KEYBOARD_BUFFER_SIZE 1024

// ----------------Data
#define MapData_1 L"Map//1-1//TileMap.png"
#define NodeTileMap_1 "Map//1-1//TileNode.txt"


// --------------------Bullet
#define PI 3.14159f
#define TimeLiveSeconds 1.0f
#define SpeedBullet 16.5f


//------ Item
#define ItemTimeAnimation 0.2f
