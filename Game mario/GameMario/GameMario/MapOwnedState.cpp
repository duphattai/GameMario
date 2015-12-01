#include "MapOwnedState.h"
#include "ReSource.h"
#include "KeyBoard.h"
#include "Camera.h"
#include "MarioOwnedState.h"
extern CKeyBoard *keyboard;

/////////////////////////////////////////////////// Bros Title
BrosTitle* BrosTitle::m_instance = 0;

BrosTitle* BrosTitle::getInstance()
{
	if (!m_instance)
		m_instance = new BrosTitle();

	return m_instance;
}

void BrosTitle::enter(MapObject* map)
{
	Camera::getInstance()->initialize(0, VIEW_PORT_Y, SCREEN_WIDTH, SCREEN_HEIGHT, 3584, 240);
	map->init(IDMap::MapOne);
}

void BrosTitle::execute(MapObject* map)
{
	keyboard->getState();
	if (keyboard->isKeyDown(DIK_SPACE))
		map->getStateMachine()->changeState(MapOne::getInstance());
}

void BrosTitle::exit(MapObject* map)
{
}

void BrosTitle::draw(MapObject* map, LPD3DXSPRITE spriteHandle)
{
	map->drawText(L"Press SPACE to play game", Vector2(90, 240 - 136));
}
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
MapOne* MapOne::m_instance = 0;

MapOne* MapOne::getInstance()
{
	if (!m_instance)
		m_instance = new MapOne();

	return m_instance;
}

void MapOne::enter(MapObject* map)
{
}

void MapOne::execute(MapObject* map)
{
	// 912, 82, 32, 15
	if (Mario::getInstance()->getFSM() == FSM_Mario::SIT && 
		AABB(Mario::getInstance()->getBouding(), Box(912, 82, 32, 15)) != DIR::NONE) // hard code, tọa độ xuống submap
	{
		Mario::getInstance()->setPosition(3355, 208);
		Mario::getInstance()->getStateMachine()->changeState(Falling::getInstance());
		
		Camera::getInstance()->setViewPort(3328, VIEW_PORT_Y);
		map->setColorBackGround(D3DCOLOR_XRGB(0, 0, 0));
	}
	else if (AABB(Mario::getInstance()->getBouding(), Box(3535, 32, 32, 15)) != DIR::NONE) // hard code, tọa độ lên map chính
	{
		Mario::getInstance()->setPosition(2608, 64);
		Mario::getInstance()->getStateMachine()->changeState(Standing::getInstance());

		Camera::getInstance()->setViewPort(2608 - SCREEN_WIDTH / 2, VIEW_PORT_Y);
		map->setColorBackGround(D3DCOLOR_XRGB(146, 144, 255));
	}
}

void MapOne::exit(MapObject* map)
{
}
/////////////////////////////////////////////////////////////////////////////////
