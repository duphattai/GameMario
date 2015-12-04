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

MapOne::MapOne()
{
	// hard code
	m_boxStartMap = Box(0, 64, 16, 16);
	m_boxEndMap = Box(3264, 32, 16, 16);

	m_boxStartSubMap = Box(3355, 208, 16, 16);
	m_boxEndSubMap = Box(3535, 32, 16, 16);
	m_boxGoInSubMap = Box(920, 82, 16, 16);
	m_boxGoOutSubMap = Box(2608, 64, 16, 16);
}

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
		Mario::getInstance()->setPosition(m_boxStartMap.x, m_boxStartMap.y);
		Camera::getInstance()->setViewPort(0, Camera::getInstance()->getViewport().y);
	}
}

void MapOne::execute(MapObject* map)
{
	// 920, 66, 16, 32
	if (Mario::getInstance()->getFSM() == FSM_Mario::SIT && 
		AABB(Mario::getInstance()->getBouding(), m_boxGoInSubMap) != DIR::NONE) // hard code, tọa độ xuống submap
	{
		if (!Mario::getInstance()->getStateMachine()->isInState(*AutoAnimation::getInstance())) // nếu chưa nằm trang thái auto animation 
		{
			AutoAnimation::getInstance()->m_type = AutoAnimationType::AutoAnimationMoveDownPipe; // thiết lập loại auto
			AutoAnimation::getInstance()->m_endPosition = Vector2(m_boxGoInSubMap.x, m_boxGoInSubMap.y); // tọa độ end auto animation
			Mario::getInstance()->getStateMachine()->changeState(AutoAnimation::getInstance());
		}
		else // trong trạng thái auto animation
		{
			if (Mario::getInstance()->isFinishAutoAnimation())
			{
				Mario::getInstance()->setPosition(m_boxStartSubMap.x, m_boxStartSubMap.y); // tọa độ mario xuất hiện trong submap
				Mario::getInstance()->getStateMachine()->changeState(Falling::getInstance());

				Camera::getInstance()->setViewPort(map->getWidth() - SCREEN_WIDTH, VIEW_PORT_Y);
				map->setColorBackGround(D3DCOLOR_XRGB(0, 0, 0));
			}
		}
	}
	else if (AABB(Mario::getInstance()->getBouding(), m_boxEndSubMap) != DIR::NONE && Mario::getInstance()->getFSM() == FSM_Mario::RUN) // tọa độ end sub map
	{
		if (!Mario::getInstance()->getStateMachine()->isInState(*AutoAnimation::getInstance()))
		{
			AutoAnimation::getInstance()->m_type = AutoAnimationType::AutoAnimationMoveOnGroundIntoPipe; // thiết lập loại auto
			AutoAnimation::getInstance()->m_endPosition = Vector2(m_boxEndSubMap.x, m_boxEndSubMap.y); // tọa độ end auto animation
			Mario::getInstance()->getStateMachine()->changeState(AutoAnimation::getInstance());
		}
		else // trong trạng thái auto animation
		{
			if (Mario::getInstance()->isFinishAutoAnimation()) // kết thúc auto animation
			{
				Mario::getInstance()->setPosition(m_boxGoOutSubMap.x, m_boxGoOutSubMap.y);
				Mario::getInstance()->getStateMachine()->changeState(Standing::getInstance());

				Camera::getInstance()->setViewPort(m_boxGoOutSubMap.x - SCREEN_WIDTH / 2, VIEW_PORT_Y);
				map->setColorBackGround(D3DCOLOR_XRGB(146, 144, 255));
			}
		}
	}
	// qua màn
	else if (Mario::getInstance()->getStateMachine()->isInState(*AutoAnimation::getInstance()) && AutoAnimation::getInstance()->m_type == AutoAnimationType::AutoAnimationMoveOnGroundIntoCastle)
	{
		AutoAnimation::getInstance()->m_endPosition = Vector2(m_boxEndMap.x, m_boxEndMap.y);
		if (Mario::getInstance()->isFinishAutoAnimation())
		{
			map->getStateMachine()->changeState(ChangeMap::getInstance());
		}
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
