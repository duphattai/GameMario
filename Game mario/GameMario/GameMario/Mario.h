#pragma once

////////////////////////////////////////////////////////////
#include "StateMachine.h"
#include <vector>
#include "Gun.h"

class Mario : public GameObject
{
private:
	StateMachine<Mario>*	m_stateMachine;
	StateMachine<Mario>*	m_statusStateMachine;

	int			m_lives; // mạng của mario

	bool		m_isBig; // trạng thái Big
	bool		m_canShoot; // Trạng thái fire
	bool		m_isStar;

	bool		m_finishAutoAnimation;

	bool		m_isDead;

	int			m_currentFrame;
	std::vector<Frame> frameList;

	Vector2		m_MaxVelocity;
	Vector2		m_MinVelocity;

	FSM_Mario	m_FSM_Mario;
	Gun*		m_gun;

	static Mario* m_instance;
	Mario();
public:
	bool		m_effectBig;
	bool		m_effectSmall;
	bool		m_effectFire;
	bool		m_unDying;


	~Mario();
	static Mario* getInstance();

	void		initialize();

	void		update();
	void		updateVelocity();
	void		draw(LPD3DXSPRITE SpriteHandler);

	StateMachine<Mario>*	getStateMachine(){ return m_stateMachine; }
	StateMachine<Mario>*	getStatusStateMachine(){ return m_statusStateMachine; }



	Box			getBouding();

	bool		isCollision(GameObject*);

	bool		isBig(){ return m_isBig; }
	void		setIsBig(bool x){ m_isBig = x; }

	bool		canShoot(){ return m_canShoot; }
	void		setCanShoot(bool x){ m_canShoot = x; }

	void		setStar(bool ability = false){ m_isStar = ability; }
	bool		isStar(){ return m_isStar; }

	void		setDead(bool x){ m_isDead = x; }
	bool		isDead(){ return m_isDead; }

	int			getLives(){ return m_lives; }
	void		setLives(int x){ m_lives = x; }

	void		setVelocity(Vector2);

	void		setFinishAutoAnimation(bool x){ m_finishAutoAnimation = x; }
	bool		isFinishAutoAnimation(){ return m_finishAutoAnimation; }

	void		setFSM(FSM_Mario state){ m_FSM_Mario = state; }
	FSM_Mario	getFSM(){ return m_FSM_Mario; }

	void		setCurrentFrame(int frame);
	int			getCurrentFrame(){ return m_currentFrame; }

	Gun*		getGun(){ return m_gun; }
};