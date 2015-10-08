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
	
	SMALL_SUPER_FIRE_STAND = 26,
	SMALL_SUPER_FIRE_RUN = 27,
	SMALL_SUPER_FIRE_CHANGE_DIR = 30,
	SMALL_SUPER_FIRE_JUMP = 31,


	BIG_INVINCIBILITY_COLOR_1_STAND = 32,
	BIG_INVINCIBILITY_COLOR_1_RUN = 33,
	BIG_INVINCIBILITY_COLOR_1_CHANGE_DIR = 36,
	BIG_INVINCIBILITY_COLOR_1_JUMP = 37,
	BIG_INVINCIBILITY_COLOR_1_SIT = 38,
	
	SMALL_INVINCIBILITY_COLOR_1_STAND = 39,
	SMALL_INVINCIBILITY_COLOR_1_RUN = 40,
	SMALL_INVINCIBILITY_COLOR_1_CHANGE_DIR = 43,
	SMALL_INVINCIBILITY_COLOR_1_JUMP = 44,



	BIG_INVINCIBILITY_COLOR_2_STAND = 45,
	BIG_INVINCIBILITY_COLOR_2_RUN = 46,
	BIG_INVINCIBILITY_COLOR_2_CHANGE_DIR = 49,
	BIG_INVINCIBILITY_COLOR_2_JUMP = 50,
	BIG_INVINCIBILITY_COLOR_2_SIT = 51,

	SMALL_INVINCIBILITY_COLOR_2_STAND = 52,
	SMALL_INVINCIBILITY_COLOR_2_RUN = 53,
	SMALL_INVINCIBILITY_COLOR_2_CHANGE_DIR = 56,
	SMALL_INVINCIBILITY_COLOR_2_JUMP = 57,
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