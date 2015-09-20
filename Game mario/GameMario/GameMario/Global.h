#pragma once

enum IDImage
{
	//IMG_UP = 0,  // 0
	//IMG_45_DEGREE, // 1
	//IMG_RIGHT_LEFT, // 2
	//IMG_315_DEGREE, //3
	//IMG_DOWN,// 4
	//IMG_STAND,// 5
	//IMG_JUMP,// 6
	//IMG_RUN_SHOOT,
	//IMG_RUN_WATER_RIGHT,
	//IMG_RUN_WATER_UP,
	//IMG_RUN_WATER_45,
	//IMG_RUN_WATER_DROP,
	//IMG_RUN_WATER_SHOOT_0,
	//IMG_PlasmaGun,
	//IMG_SpreadGun,
	//IMG_Laser,
	//IMG_MachineGun,
	//IMG_EnemyGun,
	//IMG_FireBall,
	//IMG_MAP1,
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