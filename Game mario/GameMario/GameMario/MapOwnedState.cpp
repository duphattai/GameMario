#include "MapOwnedState.h"
#include "ReSource.h"
#include "KeyBoard.h"
#include "Camera.h"
#include "MarioOwnedState.h"
#include "ScoreGame.h"
#include "GameTime.h"
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
	ReSource::getInstance()->getSprite(IDImage::IMG_MENU)->setIndex(0);
	ReSource::getInstance()->getSprite(IDImage::IMG_MENU)->draw(spriteHandle, D3DXVECTOR2(ReSource::getInstance()->getSprite(IDImage::IMG_MENU)->getWidth() / 2, ReSource::getInstance()->getSprite(IDImage::IMG_MENU)->getHeight() / 2), D3DXVECTOR2(1.0f, 1.0f), 0, VIEW_PORT_Y);
}
/////////////////////////////////////////////////////////////////////////////////


// CHANGE MAP
ChangeMap* ChangeMap::m_instance = 0;

ChangeMap* ChangeMap::getInstance()
{
	if (!m_instance)
		m_instance = new ChangeMap();

	return m_instance;
}

void ChangeMap::enter(MapObject* map)
{
	m_countTime = 30;
	map->setColorBackGround(D3DCOLOR_XRGB(0, 0, 0));
}

void ChangeMap::execute(MapObject* map)
{
	if (m_countTime-- < 0)
	{
		map->getStateMachine()->changeState(MapOne::getInstance());
	}
}

void ChangeMap::exit(MapObject* map)
{
}

void ChangeMap::draw(MapObject* map, LPD3DXSPRITE spriteHandle)
{
	string temp = "MARIO x " + to_string(Mario::getInstance()->getLives());
	map->drawText(wstring(temp.begin(), temp.end()), Vector2(90, 240 - 136));
}

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
	map->init(IDMap::MapOne);
	Mario::getInstance()->initialize();
	if (Mario::getInstance()->getPosition().x > map->getCheckPoint().x)
	{
		Mario::getInstance()->setPosition(map->getCheckPoint().x, map->getCheckPoint().y);
		Camera::getInstance()->setViewPort(map->getCheckPoint().x, Camera::getInstance()->getViewport().y);
	}
	else
	{
		Mario::getInstance()->setPosition(0, 50);
		Camera::getInstance()->setViewPort(0, Camera::getInstance()->getViewport().y);
	}
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

	// không qua màn
	if (Mario::getInstance()->getPosition().y  + Mario::getInstance()->getHeight() < 0 // mario ra khỏi màn hình
		|| ScoreGame::getInstance()->getTimeOfState() <= 0) // hết thời gian
	{
		map->getStateMachine()->changeState(ChangeMap::getInstance());
		Mario::getInstance()->setLives(Mario::getInstance()->getLives() - 1);
	}
}

void MapOne::exit(MapObject* map)
{
}
/////////////////////////////////////////////////////////////////////////////////
