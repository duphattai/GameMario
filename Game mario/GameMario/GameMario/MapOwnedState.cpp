﻿#include "MapOwnedState.h"
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
}

void BrosTitle::execute(MapObject* map)
{
	keyboard->getState();
	if (keyboard->isKeyDown(DIK_SPACE))
		map->getStateMachine()->changeState(MapThree::getInstance());
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
		if (Mario::getInstance()->getStateMachine()->isInState(*AutoAnimation::getInstance()) && AutoAnimation::getInstance()->m_type == AutoAnimationType::AutoAnimationMoveOnGroundIntoCastle)
		{
			Mario::getInstance()->setPosition(0, 0);
			map->getStateMachine()->changeState(MapTwo::getInstance());
		}
		else
			map->getStateMachine()->changeState(map->getStateMachine()->GetPreviousState());
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
	m_boxEndMap = Box(3248, 32, 16, 16);

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
	Camera::getInstance()->initialize(0, VIEW_PORT_Y, SCREEN_WIDTH, SCREEN_HEIGHT, map->getWidth(), map->getHeight());
	Mario::getInstance()->initialize();
	map->setScrollMap(true);
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
	if (Mario::getInstance()->getFSM() == FSM_Mario::SIT && 
		AABB(Mario::getInstance()->getBouding(), m_boxGoInSubMap) != DIR::NONE) // hard code, tọa độ xuống submap
	{
		if (!Mario::getInstance()->getStateMachine()->isInState(*AutoAnimation::getInstance())) 
		{
			// thiết lập auto animation đi xuống ống
			AutoAnimation::getInstance()->m_type = AutoAnimationType::AutoAnimationMoveDownPipe; // thiết lập loại auto
			AutoAnimation::getInstance()->m_endPosition = Vector2(m_boxGoInSubMap.x, m_boxGoInSubMap.y); // tọa độ end auto animation
			Mario::getInstance()->getStateMachine()->changeState(AutoAnimation::getInstance());
		}
		else // trong trạng thái auto animation
		{
			// kết thúc auto animation, đưa mario vào submap
			if (Mario::getInstance()->isFinishAutoAnimation())
			{
				map->setScrollMap(false); // trong submap không cho scroll map
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
			// thưc hiện auto animation đi vào ống
			AutoAnimation::getInstance()->m_type = AutoAnimationType::AutoAnimationMoveOnGroundIntoPipe; // thiết lập loại auto
			AutoAnimation::getInstance()->m_endPosition = Vector2(m_boxEndSubMap.x, m_boxEndSubMap.y); // tọa độ end auto animation
			Mario::getInstance()->getStateMachine()->changeState(AutoAnimation::getInstance());
		}
		else // trong trạng thái auto animation
		{
			// kết thúc auto animation, đưa mario trở lại map chính
			if (Mario::getInstance()->isFinishAutoAnimation()) 
			{
				map->setScrollMap(true);
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
		map->setScrollMap(false);
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



MapTwo* MapTwo::m_instance = 0;

MapTwo::MapTwo()
{
	// hard code
	m_boxStartMap = Box(32, 32, 16, 16);
	m_boxEndMap = Box(3760, 32, 16, 16);

	m_subMap.boxStartSubMap = Box(288, 128, 16, 16);
	m_subMap.boxEndSubMap = Box(2910, 32, 16, 16);
	m_subMap.boxGoInSubMap = Box(160, 32, 16, 16);
	m_subMap.boxGoOutSubMap = Box(3360, 64, 16, 16);

	m_subMapInSubMap.boxStartSubMap = Box(3072, 128, 16, 16);
	m_subMapInSubMap.boxEndSubMap = Box(3275, 32, 32, 16);
	m_subMapInSubMap.boxGoInSubMap = Box(1912, 64, 16, 32);
	m_subMapInSubMap.boxGoOutSubMap = Box(2096, 64, 16, 16);
}

MapTwo* MapTwo::getInstance()
{
	if (!m_instance)
		m_instance = new MapTwo();

	return m_instance;
}

void MapTwo::enter(MapObject* map)
{
	map->init(IDMap::MapTwo);
	Camera::getInstance()->initialize(0, VIEW_PORT_Y, SCREEN_WIDTH, SCREEN_HEIGHT, map->getWidth(), map->getHeight());
	if (Mario::getInstance()->getPosition().x > map->getCheckPoint().x)
	{
		Mario::getInstance()->setPosition(map->getCheckPoint().x, map->getCheckPoint().y);
		Camera::getInstance()->setViewPort(map->getCheckPoint().x, Camera::getInstance()->getViewport().y);
		map->setColorBackGround(D3DCOLOR_XRGB(0, 0, 0));
	}
	else
	{
		Mario::getInstance()->setPosition(m_boxStartMap.x, m_boxStartMap.y);
		Camera::getInstance()->setViewPort(0, Camera::getInstance()->getViewport().y);

		AutoAnimation::getInstance()->m_type = AutoAnimationType::AutoAnimationMoveOnGroundIntoPipe; // thiết lập loại auto
		AutoAnimation::getInstance()->m_endPosition = Vector2(m_subMap.boxGoInSubMap.x, m_subMap.boxGoInSubMap.y); // tọa độ end auto animation
		Mario::getInstance()->getStateMachine()->changeState(AutoAnimation::getInstance());
		map->setScrollMap(false);
	}
}

void MapTwo::execute(MapObject* map)
{
	if (AABB(Mario::getInstance()->getBouding(), m_subMap.boxGoInSubMap) != DIR::NONE) // auto animation xuống submap
	{
		if (!Mario::getInstance()->getStateMachine()->isInState(*AutoAnimation::getInstance())) // nếu chưa nằm trang thái auto animation 
		{
			AutoAnimation::getInstance()->m_type = AutoAnimationType::AutoAnimationMoveOnGroundIntoPipe; // thiết lập loại auto
			AutoAnimation::getInstance()->m_endPosition = Vector2(m_subMap.boxGoInSubMap.x, m_subMap.boxGoInSubMap.y); // tọa độ end auto animation
			Mario::getInstance()->getStateMachine()->changeState(AutoAnimation::getInstance());
		}
		else // trong trạng thái auto animation
		{
			if (Mario::getInstance()->isFinishAutoAnimation())
			{
				map->setScrollMap(true);
				// thiết lập mario trong submap
				Mario::getInstance()->setPosition(m_subMap.boxStartSubMap.x, m_subMap.boxStartSubMap.y); // tọa độ mario xuất hiện trong submap
				Mario::getInstance()->getStateMachine()->changeState(Falling::getInstance());

				Camera::getInstance()->setViewPort(256, VIEW_PORT_Y); // hard code
				map->setColorBackGround(D3DCOLOR_XRGB(0, 0, 0));
			}
		}
	}
	else if (AABB(Mario::getInstance()->getBouding(), m_subMap.boxEndSubMap) != DIR::NONE && Mario::getInstance()->getFSM() == FSM_Mario::RUN) // tọa độ end sub map
	{
		if (!Mario::getInstance()->getStateMachine()->isInState(*AutoAnimation::getInstance()))
		{
			AutoAnimation::getInstance()->m_type = AutoAnimationType::AutoAnimationMoveOnGroundIntoPipe; // thiết lập loại auto
			AutoAnimation::getInstance()->m_endPosition = Vector2(m_subMap.boxEndSubMap.x, m_subMap.boxEndSubMap.y); // tọa độ end auto animation
			Mario::getInstance()->getStateMachine()->changeState(AutoAnimation::getInstance());
		}
		else // trong trạng thái auto animation
		{
			if (Mario::getInstance()->isFinishAutoAnimation()) // kết thúc auto animation
			{
				map->setScrollMap(true);
				Mario::getInstance()->setPosition(m_subMap.boxGoOutSubMap.x, m_subMap.boxGoOutSubMap.y);
				Mario::getInstance()->getStateMachine()->changeState(Falling::getInstance());

				Camera::getInstance()->setViewPort(3328, VIEW_PORT_Y); // hard code, dựa vào image để xác định
				map->setColorBackGround(D3DCOLOR_XRGB(146, 144, 255));
			}
		}
	}

	// vào submap từ submap
	else if (AABB(Mario::getInstance()->getBouding(), m_subMapInSubMap.boxGoInSubMap) != DIR::NONE && Mario::getInstance()->getFSM() == FSM_Mario::SIT) // auto animation xuống submap từ submap
	{
		if (!Mario::getInstance()->getStateMachine()->isInState(*AutoAnimation::getInstance()))
		{
			AutoAnimation::getInstance()->m_type = AutoAnimationType::AutoAnimationMoveDownPipe; // thiết lập loại auto
			AutoAnimation::getInstance()->m_endPosition = Vector2(m_subMapInSubMap.boxGoInSubMap.x, m_subMapInSubMap.boxGoInSubMap.y); // tọa độ end auto animation
			Mario::getInstance()->getStateMachine()->changeState(AutoAnimation::getInstance());
		}
		else // trong trạng thái auto animation
		{
			if (Mario::getInstance()->isFinishAutoAnimation()) // kết thúc auto animation
			{
				map->setScrollMap(false);
				Mario::getInstance()->setPosition(m_subMapInSubMap.boxStartSubMap.x, m_subMapInSubMap.boxStartSubMap.y);
				Mario::getInstance()->getStateMachine()->changeState(Falling::getInstance());

				Camera::getInstance()->setViewPort(3072, VIEW_PORT_Y); // hard code, dựa vào image để xác định
				map->setColorBackGround(D3DCOLOR_XRGB(0, 0, 0));
			}
		}
	}
	else if (AABB(Mario::getInstance()->getBouding(), m_subMapInSubMap.boxEndSubMap) != DIR::NONE && Mario::getInstance()->getFSM() == FSM_Mario::RUN) // tọa độ end sub map trong submap
	{
		if (!Mario::getInstance()->getStateMachine()->isInState(*AutoAnimation::getInstance()))
		{
			AutoAnimation::getInstance()->m_type = AutoAnimationType::AutoAnimationMoveOnGroundIntoPipe; // thiết lập loại auto
			AutoAnimation::getInstance()->m_endPosition = Vector2(m_subMapInSubMap.boxEndSubMap.x, m_subMapInSubMap.boxEndSubMap.y); // tọa độ end auto animation
			Mario::getInstance()->getStateMachine()->changeState(AutoAnimation::getInstance());
		}
		else // trong trạng thái auto animation
		{
			if (Mario::getInstance()->isFinishAutoAnimation()) // kết thúc auto animation
			{
				map->setScrollMap(true);
				Mario::getInstance()->setPosition(m_subMapInSubMap.boxGoOutSubMap.x, m_subMapInSubMap.boxGoOutSubMap.y);
				Mario::getInstance()->getStateMachine()->changeState(Standing::getInstance());

				Camera::getInstance()->setViewPort(m_subMapInSubMap.boxGoOutSubMap.x - SCREEN_WIDTH / 2, VIEW_PORT_Y); // hard code, dựa vào image để xác định
				map->setColorBackGround(D3DCOLOR_XRGB(0, 0, 0));
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
	if (Mario::getInstance()->getPosition().y + Mario::getInstance()->getHeight() < 0 // mario ra khỏi màn hình
		|| ScoreGame::getInstance()->getTimeOfState() <= 0) // hết thời gian
	{
		map->getStateMachine()->changeState(ChangeMap::getInstance());
		Mario::getInstance()->setLives(Mario::getInstance()->getLives() - 1);
	}
}

void MapTwo::exit(MapObject* map)
{
}
/////////////////////////////////////////////////////////////////////////////////

MapThree* MapThree::m_instance = 0;

MapThree::MapThree()
{
	// hard code
	m_boxStartMap = Box(32, 64, 16, 16);
	m_boxEndMap = Box(2544, 32, 16, 16);
}

MapThree* MapThree::getInstance()
{
	if (!m_instance)
		m_instance = new MapThree();

	return m_instance;
}

void MapThree::enter(MapObject* map)
{
	map->init(IDMap::MapThree);
	Camera::getInstance()->initialize(0, VIEW_PORT_Y, SCREEN_WIDTH, SCREEN_HEIGHT, map->getWidth(), map->getHeight());
	Mario::getInstance()->initialize();
	map->setScrollMap(true);
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

void MapThree::execute(MapObject* map)
{
	if (Mario::getInstance()->getStateMachine()->isInState(*AutoAnimation::getInstance()) && AutoAnimation::getInstance()->m_type == AutoAnimationType::AutoAnimationMoveOnGroundIntoCastle)
	{
		map->setScrollMap(false);
		AutoAnimation::getInstance()->m_endPosition = Vector2(m_boxEndMap.x, m_boxEndMap.y);
		if (Mario::getInstance()->isFinishAutoAnimation())
		{
			map->getStateMachine()->changeState(ChangeMap::getInstance());
		}
	}

	// không qua màn
	if (Mario::getInstance()->getPosition().y + Mario::getInstance()->getHeight() < 0 // mario ra khỏi màn hình
		|| ScoreGame::getInstance()->getTimeOfState() <= 0) // hết thời gian
	{
		map->getStateMachine()->changeState(ChangeMap::getInstance());
		Mario::getInstance()->setLives(Mario::getInstance()->getLives() - 1);
	}
}

void MapThree::exit(MapObject* map)
{
}