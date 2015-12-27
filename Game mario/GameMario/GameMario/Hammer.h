#pragma once
#include "GameObject.h"
#include "StateMachine.h"
#include <vector>

class Hammer : public GameObject
{
	StateMachine<Hammer>*		m_stateMachine;
	// time live of bullet
	int							m_timeToLive;
	int							m_currentFrame;
	vector<Frame>				m_frameList;
	bool						m_isExplode;


public:
	float						time;
	float						alpha;
	Hammer();
	~Hammer();

	void						shoot(SpriteEffect flip, Vector2 position, Vector2 worldPosition);

	void						setExplode(bool x){ m_isExplode = x; }
	bool						isExplode(){ return m_isExplode; }

	bool						isCollision(GameObject*);

	StateMachine<Hammer>*		getStateMachine(){ return m_stateMachine; }

	int							getCurrentFrame(){ return m_currentFrame; }
	void						setCurrentFrame(int frame);

	int							getSizeOfFrameList(){ return m_frameList.size(); }

	void						setTimeToLive(int time){ m_timeToLive = time; }
	int							getTimeToLive(){ return m_timeToLive; }

	void						updateVelocity();
	void						update();
	void						draw(LPD3DXSPRITE SpriteHandler);
};

