#pragma once
#include "stdafx.h"

enum IDImage
{
	IMG_MARIOSHEET = 0,
	IMG_TILEMAP,
	IMG_ITEMSHEET,
	IMG_MENU,
	IMG_ENEMYSHEET
};

enum SpriteEffect
{
	None = 0,
	Flip,
};

enum Location
{
	LOC_ON_GROUND,
	LOC_IN_AIR
};

enum MarioSheet
{
	BIG_MARIO_STAND = 0,
	BIG_MARIO_RUN = 1,
	BIG_MARIO_CHANGE_DIR = 4,
	BIG_MARIO_JUMP = 5,
	BIG_MARIO_SIT = 6,
	BIG_SMALL_STAND = 7,

	MARIO_STAND = 8,
	MARIO_RUN = 9,
	MARIO_CHANGE_DIR = 12,
	MARIO_JUMP = 13,
	MARIO_DIE = 14,
	


	BIG_SUPER_STAND = 15,
	BIG_SUPER_RUN = 16,
	BIG_SUPER_CHANGE_DIR = 19,
	BIG_SUPER_JUMP = 20,
	BIG_SUPER_SIT = 21,
	BIG_SUPER_FIRE_RUN = 22,
	BIG_SUPER_FIRE_CHANGE_DIR = 25,
	BIG_SUPER_FIRE_JUMP = 26,

	SMALL_SUPER_FIRE_STAND = 27,
	SMALL_SUPER_FIRE_RUN = 28,
	SMALL_SUPER_FIRE_CHANGE_DIR = 31,
	SMALL_SUPER_FIRE_JUMP = 32,


	BIG_INVINCIBILITY_COLOR_1_STAND = 33,
	BIG_INVINCIBILITY_COLOR_1_RUN = 34,
	BIG_INVINCIBILITY_COLOR_1_CHANGE_DIR = 37,
	BIG_INVINCIBILITY_COLOR_1_JUMP = 38,
	BIG_INVINCIBILITY_COLOR_1_SIT = 39,
	
	SMALL_INVINCIBILITY_COLOR_1_STAND = 40,
	SMALL_INVINCIBILITY_COLOR_1_RUN = 41,
	SMALL_INVINCIBILITY_COLOR_1_CHANGE_DIR = 44,
	SMALL_INVINCIBILITY_COLOR_1_JUMP = 45,



	BIG_INVINCIBILITY_COLOR_2_STAND = 46,
	BIG_INVINCIBILITY_COLOR_2_RUN = 47,
	BIG_INVINCIBILITY_COLOR_2_CHANGE_DIR = 50,
	BIG_INVINCIBILITY_COLOR_2_JUMP = 51,
	BIG_INVINCIBILITY_COLOR_2_SIT = 52,

	SMALL_INVINCIBILITY_COLOR_2_STAND = 53,
	SMALL_INVINCIBILITY_COLOR_2_RUN = 54,
	SMALL_INVINCIBILITY_COLOR_2_CHANGE_DIR = 57,
	SMALL_INVINCIBILITY_COLOR_2_JUMP = 58,
};

enum class IDMap
{
	MapOne = 0,
	MapTwo,
	MapThree
};

struct Box
{
	float x, y;
	float width, height;

	float vx, vy;

	Box(){}

	Box(float x, float y, float w, float h, float vx = 0.0f, float vy = 0.0f)
	{
		this->x = x;
		this->y = y;

		width = w;
		height = h;

		this->vx = vx;
		this->vy = vy;
	}
};

enum DIR
{
	NONE = 0,
	LEFT,
	RIGHT,
	TOP,
	BOTTOM
};

enum FSM_Mario
{
	STAND = 0,
	RUN,
	JUMP,
	FALL,
	SIT,
	DEAD,
	STAR
};

enum TypeObject
{
	Moving_Enemy,
	Moving_Item,
	Dynamic_Item,
	Dynamic_TiledMap,
	Dynamic_StandPosition,
};

enum LuckyBoxsType
{
	IT_COIN = 0,
	IT_MUSHROOM_BIGGER,
	IT_MUSHROOM_UP,
	IT_GUN,
	IT_STAR
};

enum class ItemTypes
{
	BrickLuckyBox,
	YellowLuckyBox,
	Brick
};

struct Frame
{
	int id;
	RECT rect;
public:
	Frame(int id, RECT rect)
	{
		this->id = id;
		this->rect = rect;
	}
	Frame(int x, int y, int width, int height)
	{
		rect.left = x;
		rect.right = rect.left + width;
		rect.top = y;
		rect.bottom = rect.top + height;
	}
};

struct Vector2
{
	float x;
	float y;
	Vector2(float x = 0, float y = 0)
	{
		this->x = x;
		this->y = y;
	}
	Vector2 operator+(const Vector2& temp)
	{
		return Vector2(this->x + temp.x, this->y + temp.y);
	}
};

struct ObjectTittle
{
	int m_Index;
	int m_X;
	int m_Y;
	int m_Id;
	int m_Width;
	int m_Height;

	ObjectTittle(int id = 0, int index = 0, int x = 0, int y = 0, int width = 0, int height = 0)
	{
		m_Id = id;
		m_Index = index;
		m_X = x;
		m_Y = y;
		m_Width = width;
		m_Height = height;
	}

	bool operator==(const ObjectTittle& temp)
	{
		if (temp.m_Id != m_Id || temp.m_Height != m_Height || temp.m_Index != m_Index ||
			temp.m_Width != m_Width || temp.m_X != m_X || temp.m_Y != m_Y)
			return false;

		return true;
	}
};

enum class StatusObject
{
	ALIVE,
	DEAD
};

static Vector2 vanTocNemXien(int time, float v, float alpha)
{
	Vector2 velocity;
	velocity.x = v * cos(alpha);
	velocity.y = v * sin(alpha) + GRAVITATION * time / 1.5;

	return velocity;
}

enum class IDSounds
{
	Sound_1_Up,
	Sound_BreakBlock,
	Sound_Coin,
	Sound_FireBall,
	Sound_FireWorks,
	Sound_GameOver,
	Sound_Jump_Small,
	Sound_Jump_Super,
	Sound_Kick,
	Sound_MarioDie,
	Sound_Pause,
	Sound_Pipe,
	Sound_PowerUp,
	Sound_PowerUp_Appears,
	Sound_StageCLear,
	Sound_Stomp,
	Sound_Vine,
	Sound_Warning,
	Sound_WorldCLear,
	Sound_FlagPole
};
