#include "Map1.h"
#include "Source.h"
#include "SwapAABB.h"
Map1::Map1(Mario *mario)
{
	m_Sprite = Source::getInstance()->getSprite(IDImage::IMG_MAP1);
	m_WorldPosition.y = VIEW_PORT_Y;

	Line = 3;
	m_Mario = mario;
	m_CheckCollision = new Collision();
}

void Map1::init(CKeyBoard *keyboard)
{
	MapObject::init(keyboard, NodeTileMap_1);
}


void Map1::updateCollision()
{
	// collision with map
	// mario
	if (m_Mario->getFSM() != FSM_Mario::DEAD)
	{
		m_Mario->setDirCollision(DIR::NONE);
		m_Mario->setLocation(Location::LOC_IN_AIR);
		Box marioBound = GetSweptBroadPhaseBox(m_Mario->getBouding()); // tao vung khong gian cho mario
		for each (Box item in getTileNodeOnScreen())
		{
			// kiem tra item co nam trong vung khong gian cua mario
			if (CheckAABB(marioBound, item))
			{
				DIR dir = m_CheckCollision->isCollision(m_Mario, item);
				if (dir != DIR::NONE)
				{
					if (m_Mario->getFSM() != FSM_Mario::JUMP && dir == DIR::TOP) // fall gặp vật cản
						m_Mario->setLocation(Location::LOC_ON_GROUND);
				}

				if (m_Mario->getDirCollision() == DIR::NONE)
					m_Mario->setDirCollision(dir);
			}
		}
	}
}
void Map1::update()
{
	m_keyboard->getState();
	if (m_keyboard->isPressed(DIK_SPACE))
		m_Mario->setIsBig(true);
	else if (m_keyboard->isPressed(DIK_C))
		m_Mario->setCanShoot(true);
	else if (m_keyboard->isPressed(DIK_K))
		m_Mario->setDead(true);
	else if (m_keyboard->isPressed(DIK_V))
		m_Mario->setStar(true);
}

vector<ObjectTittle> getNodeOnCamera(vector<ObjectTittle> listNode, Box camera)
{
	vector<ObjectTittle> list;
	for each(ObjectTittle item in listNode)
	{
		Box box(item.m_X, item.m_Y, item.m_Width, item.m_Height);
		if (CheckAABB(box, camera))
			list.push_back(item);
	}

	return list;
}

void Map1::draw(LPD3DXSPRITE SpriteHandler)
{
	vector<ObjectTittle> list = getNodeOnCamera(m_tileMapNode, Box(m_WorldPosition.x, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	
	for each(ObjectTittle item in list)
	{
		if (item.m_Id == 0)
		{
			m_Sprite->setIndex(item.m_Index);
			m_Position.x = (float)item.m_X;
			m_Position.y = (float)item.m_Y;
			Object::draw(SpriteHandler);
		}
	}
}

Map1::~Map1()
{
}
