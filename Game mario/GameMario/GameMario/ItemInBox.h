#pragma once
#include "GameObject.h"
#include "StateMachine.h"
#include <vector>

enum ItemSheet
{
	IS_MUSHROOM_BIGGER = 0,
	IS_MUSHROOM_UP = 1,
	IS_GUNFLOWER = 2,
	IS_STAR = 6,
	IS_COIN = 10,
	IS_LUCKYBOX = 18,
	IS_BREAKBRICK = 22,
	IS_BULLET = 25,
	IS_BULLET_EXPLODE = 29,
	IS_FLAG = 32
};

enum ItemsType
{
	IT_COIN = 0,
	IT_MUSHROOM_BIGGER,
	IT_MUSHROOM_UP,
	IT_GUN,
	IT_STAR,
	IT_BRICK
};

class ItemInBox : public GameObject
{
private:
	StateMachine<ItemInBox>*	m_stateMachine;

	ItemsType					m_type;
	bool						m_finishAnimation;

	int							m_currentFrame;
	std::vector<Frame>			m_frameList;

	int							m_timeAnimation;

	bool						m_isCollision;
	bool						m_isActive;
public:
	ItemInBox(std::vector<Frame> frameList, ItemsType type);
	~ItemInBox();

	StateMachine<ItemInBox>*	getStateMachine(){ return m_stateMachine; };

	void		update();
	void		updateVelocity();
	void		draw(LPD3DXSPRITE SpriteHandler);

	void		setCurrentFrame(int frame){ m_currentFrame = frame; m_Sprite->setRect(m_frameList[m_currentFrame].rect); }
	int			getCurrentFrame(){ return m_currentFrame; }

	int			getSizeFrameList(){ return m_frameList.size(); }

	bool		finishAnimation(){ return m_finishAnimation; }
	void		setFinishAnimation(bool x){ m_finishAnimation = x; }

	void		setTimeAnimation(int x){ m_timeAnimation = x; }
	int			getTimeAnimation(){ return m_timeAnimation; }

	bool		getCollision(){ return m_isCollision; }
	void		setCollision(bool x){ m_isCollision = x; }

	void		setItemsType(ItemsType type);

	void		setActive(bool x){ m_isActive = x; }
	bool		isActive(){ return m_isActive; }

};

