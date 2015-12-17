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

enum EnemyType
{
	TortoiseEnemy,
	MushroomEnemy,
	CloudEnemy,
	FlowerEnemy,

};
class Enemy : public GameObject
{
protected:
	StateMachine<Enemy>*		m_stateMachine;
	Vector2						m_positionText;
	BeAttack					m_beAttack;

	int							m_currentFrame;
	vector<Frame>				m_frameList;

	EnemyType					m_enemyType;

public:

	void					setCurrentFrame(int frame);
	int						getCurrentFrame(){ return m_currentFrame; }

	bool					isCollision(GameObject*);

	void					update();
	void					updateVelocity();
	void					draw(LPD3DXSPRITE SpriteHandler);

	int						getSizeFrameList(){ return m_frameList.size(); }
	void					setFrameList(vector<Frame> list){ m_frameList = list; }

	EnemyType				getType(){ return m_enemyType; }

	BeAttack				getAttack(){ return m_beAttack; }
	void					setAttack(BeAttack beAttack){ m_beAttack = beAttack; }

	void					setPositionText(Vector2 position){ m_positionText = position; }

	StateMachine<Enemy>*	getStateMachine(){ return m_stateMachine; }

	Enemy(EnemyType newEnemyType);
	~Enemy();
};