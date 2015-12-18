#pragma once
#include "GameObject.h"
#include <vector>
#include "StateMachine.h"


enum class BeAttack
{
	None,
	DeathByJump,
	DeathByGun
};

class Enemy : public GameObject
{
protected:
	StateMachine<Enemy>*		m_stateMachine;
	Vector2						m_positionText;
	BeAttack					m_beAttack;

	int							m_currentFrame;
	vector<Frame>				m_frameList;
public:
	void					setCurrentFrame(int frame);
	int						getCurrentFrame(){ return m_currentFrame; }

	void					update();
	void					updateVelocity();
	void					draw(LPD3DXSPRITE SpriteHandler);

	virtual Box				getBouding();

	int						getSizeFrameList(){ return m_frameList.size(); }
	void					setFrameList(vector<Frame> list){ m_frameList = list; }

	BeAttack				getAttack(){ return m_beAttack; }
	void					setAttack(BeAttack beAttack){ m_beAttack = beAttack; }

	void					setPositionText(Vector2 position){ m_positionText = position; }

	StateMachine<Enemy>*	getStateMachine(){ return m_stateMachine; }

	Enemy();
	~Enemy();
};