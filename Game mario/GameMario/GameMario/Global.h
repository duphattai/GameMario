#pragma once

enum IDImage
{
	IMG_MARIOSHEET = 0,
	IMG_MAP1,
	//IMG_MAP2,
	//IMG_HumandExplode,
	//IMG_MachineExplode,
	//IMG_BlueExplode,
	//IMG_ItemBLock,
	//IMG_ItemGun,
	//IMG_BlockTank,
	//IMG_BlockTank2,
	//IMG_Bridge,
	//IMG_SoldierRun,
	//IMG_SoldierJump,
	//IMG_SoldierSniper_45,
	//IMG_SoldierSniper_90,
	//IMG_SoldierSniper_135,
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
	SMALL_BIG_STAND = 7,
	MARIO_STAND = 8,
	MARIO_RUN = 9,
	MARIO_CHANGE_DIR = 12,
	MARIO_JUMP = 13,
	MARIO_DIE = 14,
	SUPER_MARIO_STAND = 15,
	SUPER_MARIO_RUN = 16,
	SUPER_MARIO_CHANGE_DIR = 19,
	SUPER_MARIO_JUMP = 20,
	SUPER_MARIO_SIT = 21,
	SUPER_MARIO_FIRE_RUN = 22,
	INVINCIBILITY_COLOR_1_STAND = 25,
	INVINCIBILITY_COLOR_1_RUN = 26,
	INVINCIBILITY_COLOR_1_CHANGE_DIR = 29,
	INVINCIBILITY_COLOR_1_JUMP = 30,
	INVINCIBILITY_COLOR_1_SIT = 31,
	INVINCIBILITY_COLOR_2_STAND = 32,
	INVINCIBILITY_COLOR_2_RUN = 33,
	INVINCIBILITY_COLOR_2_CHANGE_DIR = 36,
	INVINCIBILITY_COLOR_2_JUMP = 37,
	INVINCIBILITY_COLOR_2_SIT = 38,
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
	EFFECT_BIG,
	EFFECT_FIRE,
	EFFECT_SMALL,
	DEAD
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
};