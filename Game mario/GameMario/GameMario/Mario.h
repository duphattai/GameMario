#pragma once

////////////////////////////////////////////////////////////
#include "StateMachine.h"
#include "Object.h"
#include <vector>


class Mario : public Object
{
private:
	StateMachine<Mario>*	m_stateMachine;

	int			m_lives;

	bool		m_isBig;
	bool		m_canShoot;
	bool		m_canJump;

	bool		m_isBoost;

	int			m_currentFrame;
	std::vector<Frame> frameList;

	Vector2		m_MaxVelocity;
	Vector2		m_MinVelocity;

	FSM_Mario	m_FSM_Mario;
	//Gun*		m_gun;
public:
	Mario();
	~Mario();

	void		update();
	void		updateVelocity();
	void		draw(LPD3DXSPRITE SpriteHandler);

	StateMachine<Mario>*	getStateMachine(){ return m_stateMachine; }

	bool		isBig(){ return m_isBig; }
	void		setIsBig(bool x);

	Vector2		getMaxVelocity(){ return m_MaxVelocity; }
	Vector2		getMinVelocity(){ return m_MinVelocity; }

	bool		canShoot(){ return m_canShoot; }
	void		setCanShoot(bool x){ m_canShoot = x; }

	void		jump(bool ability = false){ m_canJump = ability; }
	bool		canJump(){ return m_canJump; }

	bool		isBoost(){ return m_isBoost; }

	void		setVelocity(Vector2);

	void		setFSM(FSM_Mario state){ m_FSM_Mario = state; }
	FSM_Mario	getFSM(){ return m_FSM_Mario; }

	void		setCurrentFrame(int frame){ m_currentFrame = frame; }
	int			getCurrentFrame(){ return m_currentFrame; }
	//Gun*		getGun(){ return m_gun; }
};