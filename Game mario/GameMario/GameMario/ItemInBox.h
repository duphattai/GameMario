#pragma once
#include "GameObject.h"
#include "StateMachine.h"
#include <vector>
using namespace std;

Vector2 vanTocNemXien(int time, float v, float alpha);

class ItemInBox : public GameObject
{
private:
	StateMachine<ItemInBox>*	m_stateMachine;

	ItemsType					m_type;
	bool						m_finishAnimation;

	int							m_currentFrame;
	vector<Frame>				m_frameList;
public:
	ItemInBox(ItemsType type);
	~ItemInBox();

	StateMachine<ItemInBox>*	getStateMachine(){ return m_stateMachine; };

	bool					isCollision(GameObject*);

	Box			getBouding();

	void		update();
	void		updateVelocity();
	void		draw(LPD3DXSPRITE SpriteHandler);

	void		setCurrentFrame(int frame){ m_currentFrame = frame; m_sprite->setRect(m_frameList[m_currentFrame].rect); }
	int			getCurrentFrame(){ return m_currentFrame; }

	int			getSizeFrameList(){ return m_frameList.size(); }

	bool		finishAnimation(){ return m_finishAnimation; }
	void		setFinishAnimation(bool x){ m_finishAnimation = x; }

	void		setItemsType(ItemsType type);
	ItemsType	getItemsType(){ return m_type; }

	int			m_time; // thời gian xác định vận tốc
	Vector2(*m_mathematical)(int, float v, float alpha);// con trỏ hàm
};

