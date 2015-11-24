#include "MapOwnedState.h"
#include "ReSource.h"
#include "KeyBoard.h"

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
	map->init(IDMap::MapOne);
}

void BrosTitle::execute(MapObject* map)
{
	keyboard->getState();
	if (keyboard->isPressed(DIK_SPACE))
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
}

void MapOne::exit(MapObject* map)
{
	map->release();
}
/////////////////////////////////////////////////////////////////////////////////
