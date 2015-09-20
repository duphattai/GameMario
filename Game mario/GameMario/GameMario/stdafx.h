#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include <dinput.h>
#include "Global.h"

// -----------------------windowed
#define SCREEN_WIDTH 250
#define SCREEN_HEIGHT 232
#define MAIN_WINDOW_TITLE "Sample"
#define FPS 30

// -------------------character
#define SPEED 4.5f
#define VECLOCITY_VY 12.5f
#define FALLDOWN_VELOCITY_DECREASE 15.0f
#define GROUND_Y 100
#define VIEW_PORT_Y 232

// xet id phu thuoc vao file text quy dinh
// 0.* = di chuyen tren mat dat  



//-----------------------------key board
#pragma comment (lib,"dxguid.lib") 
#define KEYBOARD_BUFFER_SIZE 1024

// ----------------Data
#define MapData_1 "Map//TileMap1.png"
#define NodeTileMap_1 "Map//TileNode1.txt"


// --------------------Bullet
#define PI 3.14159f
#define TimeLiveSeconds 1.0f
#define SpeedBullet 16.5f

//------ Item
#define ItemTimeAnimation 0.2f
