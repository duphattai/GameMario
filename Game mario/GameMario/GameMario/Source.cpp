#include "Source.h"

Source* Source::m_Instance = NULL;

Source::Source()
{}

void Source::init(LPDIRECT3DDEVICE9 d3ddv)
{
	//m_Attack_left_Right = new Sprite();
	//m_Attack_left_Right->loadImageFromPath(d3ddv, "Character//90.png", 2, 2, D3DCOLOR_XRGB(255, 255, 0));

	//m_BlueMove_Left_Right = new Sprite();
	//m_BlueMove_Left_Right->loadImageFromPath(d3ddv, "Character//Run_Right.png", 5, 5, D3DCOLOR_XRGB(255, 255, 0));

	//m_Blue_Up = new Sprite();
	//m_Blue_Up->loadImageFromPath(d3ddv, "Character//Up.png", 2, 2, D3DCOLOR_XRGB(255, 255, 0));

	//m_Blue_Down = new Sprite();
	//m_Blue_Down->loadImageFromPath(d3ddv, "Character//Down.png", 2, 2, D3DCOLOR_XRGB(255, 255, 0));

	//m_Move_315_225 = new Sprite();
	//m_Move_315_225->loadImageFromPath(d3ddv, "Character//135.png", 3, 3, D3DCOLOR_XRGB(255, 255, 0));

	//m_Move_45_135 = new Sprite();
	//m_Move_45_135->loadImageFromPath(d3ddv, "Character//45.png", 3, 3, D3DCOLOR_XRGB(255, 255, 0));

	//m_Blue_Jump = new Sprite();
	//m_Blue_Jump->loadImageFromPath(d3ddv, "Character//Jump_Right.png", 4, 4, D3DCOLOR_XRGB(255, 255, 0));

	//m_Blue_Run_Shoot = new Sprite();
	//m_Blue_Run_Shoot->loadImageFromPath(d3ddv, "Character//Run_Shoot.png", 3, 3, D3DCOLOR_XRGB(255, 255, 0));

	//m_Stand = new Sprite();
	//m_Stand->loadImageFromPath(d3ddv, "Character//Stand.png", 2, 2, D3DCOLOR_XRGB(255, 255, 0));

	//// blue move under water
	//m_BlueMoveWater_Right = new Sprite();
	//m_BlueMoveWater_Right->loadImageFromPath(d3ddv, "Character//Water_Run.png", 2, 2, D3DCOLOR_XRGB(34, 177, 76));

	//m_BlueMoveWater_45 = new Sprite();
	//m_BlueMoveWater_45->loadImageFromPath(d3ddv, "Character//Water_Shoot_45.png", 2, 2, D3DCOLOR_XRGB(255, 255, 0));

	//m_BlueMoveWater_Drop = new Sprite();
	//m_BlueMoveWater_Drop->loadImageFromPath(d3ddv, "Character//Water_Drop.png", 2, 2, D3DCOLOR_XRGB(255, 255, 0));

	//m_BlueMoveWater_Up = new Sprite();
	//m_BlueMoveWater_Up->loadImageFromPath(d3ddv, "Character//Water_Shoot_0.png", 2, 2, D3DCOLOR_XRGB(255, 255, 0));

	//m_BlueMoveWater_Shoot_0 = new Sprite();
	//m_BlueMoveWater_Shoot_0->loadImageFromPath(d3ddv, "Character//Water_Shoot_90.png", 2, 2, D3DCOLOR_XRGB(255, 255, 0));

	//// Gun in game
	//m_PlasmaGun = new Sprite();
	//m_PlasmaGun->loadImageFromPath(d3ddv, "Bullet//Bullet0.png", 1, 1, D3DCOLOR_XRGB(255, 255, 0));

	//m_MachineGun = new Sprite();
	//m_MachineGun->loadImageFromPath(d3ddv, "Bullet//Bullet1.png", 1, 1, D3DCOLOR_XRGB(255, 255, 0));

	//m_SpreadGun = m_MachineGun;
	//m_EnemyGun = m_PlasmaGun;

	//// Map
	//m_Map1 = new Sprite();
	///*m_Map1->loadImageFromPath(d3ddv, MapData_1, 122, 122, D3DCOLOR_XRGB(255, 0, 255));*/
	//m_Map1->loadImageFromPath(d3ddv, MapData_1, 91, 91, D3DCOLOR_XRGB(255, 0, 255));

	//// Explode
	//m_BlueDie = new Sprite();
	//m_BlueDie->loadImageFromPath(d3ddv, "Character//Die_Right.png", 5, 5, D3DCOLOR_XRGB(255, 0, 255));

	//m_MaChineExplode = new Sprite();
	//m_MaChineExplode->loadImageFromPath(d3ddv, "Character//Burst_Item_BlockHouse.png", 3, 3, D3DCOLOR_XRGB(255, 0, 255));

	//m_HumandExplode = new Sprite();
	//m_HumandExplode->loadImageFromPath(d3ddv, "Character//Burst_Enemy.png", 3, 3, D3DCOLOR_XRGB(255, 0, 255));

	//// Item
	//m_Item_Block = new Sprite();
	//m_Item_Block->loadImageFromPath(d3ddv, "Item//ItemBlock.png", 7, 7, D3DCOLOR_XRGB(255, 0, 255));

	//m_Item_Gun = new Sprite();
	//m_Item_Gun->loadImageFromPath(d3ddv, "Item//ItemGun.png", 6, 6, D3DCOLOR_XRGB(255, 255, 255));

	//// Tank 
	//m_Block_Tank = new Sprite();
	//m_Block_Tank->loadImageFromPath(d3ddv, "Block Tank//BlockTank.png", 9, 9, D3DCOLOR_XRGB(255, 0, 255));
	//m_Block_Tank2 = new Sprite();
	//m_Block_Tank2->loadImageFromPath(d3ddv, "Block Tank//BlockTank2.png", 5, 5, D3DCOLOR_XRGB(255, 0, 255));

	//// Soldier
	//m_Soldier_Run = new Sprite();
	//m_Soldier_Run->loadImageFromPath(d3ddv, "Enemy//Soldier_Run.png", 5, 5, D3DCOLOR_XRGB(255, 0, 255));
	//m_Soldier_Jump = new Sprite();
	//m_Soldier_Jump->loadImageFromPath(d3ddv, "Enemy//Soldier_Jump.png", 1, 1, D3DCOLOR_XRGB(255, 0, 255));

	//// SoldierSniper
	//m_SoldierSniper_45 = new Sprite();
	//m_SoldierSniper_45->loadImageFromPath(d3ddv, "Enemy//SoldierAI_45.png", 2, 2, D3DCOLOR_XRGB(255, 0, 255));
	//m_SoldierSniper_90 = new Sprite();
	//m_SoldierSniper_90->loadImageFromPath(d3ddv, "Enemy//SoldierAI_90.png", 2, 2, D3DCOLOR_XRGB(255, 0, 255));
	//m_SoldierSniper_135 = new Sprite();
	//m_SoldierSniper_135->loadImageFromPath(d3ddv, "Enemy//SoldierAI_135.png", 1, 1, D3DCOLOR_XRGB(255, 0, 255));
	//// Bridge
	//m_Bridge = new Sprite();
	//m_Bridge->loadImageFromPath(d3ddv, "Map//Bridge.png", 4, 4, D3DCOLOR_XRGB(255, 0, 255));
}

//Sprite* Source::getSprite(int Id)
//{
//	/*switch (Id)
//	{
//	case IDImage::IMG_DOWN:
//		return m_Blue_Down;
//	case IDImage::IMG_RIGHT_LEFT:
//		return m_BlueMove_Left_Right;
//	case IDImage::IMG_UP:
//		return m_Blue_Up;
//	case IDImage::IMG_315_DEGREE:
//		return m_Move_315_225;
//	case IDImage::IMG_45_DEGREE:
//		return m_Move_45_135;
//	case IDImage::IMG_STAND:
//		return m_Stand;
//	case IDImage::IMG_JUMP:
//		return m_Blue_Jump;
//	case IDImage::IMG_RUN_SHOOT:
//		return m_Blue_Run_Shoot;
//
//
//	case IDImage::IMG_PlasmaGun:
//		return m_PlasmaGun;
//	case IMG_SpreadGun:
//		return m_SpreadGun;
//	case IMG_MachineGun:
//		return m_MachineGun;
//	case IMG_EnemyGun:
//		return m_EnemyGun;
//
//	case IDImage::IMG_MAP1:
//		return m_Map1;
//	case IDImage::IMG_MAP2:
//		break;
//	case IDImage::IMG_Bridge:
//		return m_Bridge;
//	case IDImage::IMG_HumandExplode:
//		return m_HumandExplode;
//	case IDImage::IMG_MachineExplode:
//		return m_MaChineExplode;
//	case IDImage::IMG_BlueExplode:
//		return m_BlueDie;
//	case IDImage::IMG_RUN_WATER_45:
//		return m_BlueMoveWater_45;
//	case IDImage::IMG_RUN_WATER_DROP:
//		return m_BlueMoveWater_Drop;
//	case IDImage::IMG_RUN_WATER_RIGHT:
//		return m_BlueMoveWater_Right;
//	case IDImage::IMG_RUN_WATER_UP:
//		return m_BlueMoveWater_Up;
//	case IDImage::IMG_RUN_WATER_SHOOT_0:
//		return m_BlueMoveWater_Shoot_0;
//	case IDImage::IMG_ItemBLock:
//		return m_Item_Block;
//	case IDImage::IMG_ItemGun:
//		return m_Item_Gun;
//	case IDImage::IMG_BlockTank:
//		return m_Block_Tank;
//	case IDImage::IMG_BlockTank2:
//		return m_Block_Tank2;
//
//	case IDImage::IMG_SoldierRun:
//		return m_Soldier_Run;
//	case IDImage::IMG_SoldierJump:
//		return m_Soldier_Jump;
//
//	case IDImage::IMG_SoldierSniper_45:
//		return m_SoldierSniper_45;
//	case IDImage::IMG_SoldierSniper_90:
//		return m_SoldierSniper_90;
//	case IDImage::IMG_SoldierSniper_135:
//		return m_SoldierSniper_135;
//	}*/
//}

Source* Source::getInstance()
{
	if (m_Instance == NULL)
		m_Instance = new Source();

	return m_Instance;
}

Source::~Source()
{
}
