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


enum MarioSheet
{
	BIG_MARIO_STAND = 0,
	BIG_MARIO_RUN = 1,
	BIG_MARIO_CHANGE_DIR = 4,
	BIG_MARIO_JUMP = 5,
	BIG_MARIO_SIT = 6,
	MARIO_STAND = 7,
	MARIO_RUN = 8,
	MARIO_CHANGE_DIR = 11,
	MARIO_JUMP = 12,
	MARIO_DIE = 13,
	SUPER_MARIO_STAND = 14,
	SUPER_MARIO_RUN = 15,
	SUPER_MARIO_CHANGE_DIR = 18,
	SUPER_MARIO_JUMP = 19,
	SUPER_MARIO_SIT = 20,
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
	int x;
	int y;
	Vector2(int x = 0, int y = 0)
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