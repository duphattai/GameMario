#include "Map1.h"
#include "Source.h"
#include "SwapAABB.h"
Map1::Map1()
{
	m_Sprite = Source::getInstance()->getSprite(IDImage::IMG_MAP1);
	m_WorldPosition.y = VIEW_PORT_Y;

	Line = 2;
}

void Map1::init(CKeyBoard *keyboard)
{
	MapObject::init(keyboard, NodeTileMap_1);
}

void Map1::update()
{
	m_keyboard->getState();
	if (m_keyboard->isKeyDown(DIK_RIGHT))
		m_WorldPosition.x += 5;
	else if (m_keyboard->isKeyDown(DIK_LEFT))
		m_WorldPosition.x -= 5;
	
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
			m_Position.x = item.m_X;
			m_Position.y = item.m_Y;
			Object::draw(SpriteHandler);
		}
	}
}

Map1::~Map1()
{
}
