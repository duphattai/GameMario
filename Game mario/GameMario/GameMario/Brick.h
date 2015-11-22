#pragma once
#include "GameObject.h"
#include <vector>
#include "StateMachine.h"

using namespace std;

class Brick : public GameObject
{
	StateMachine<Brick>*		m_stateMachine;

	//ItemsType					m_type;
	bool						m_isBreak;

	int							m_currentFrame;
	vector<Frame>				m_frameList;

	vector<Vector2>				m_framePosition; // tọa độ mini brick
	
	bool						m_makeEffect;
public:
	Brick(Vector2);
	~Brick();

	StateMachine<Brick>*	getStateMachine(){ return m_stateMachine; }

	void					setCurrentFrame(int frame){ m_currentFrame = frame; m_sprite->setRect(m_frameList[m_currentFrame].rect); }
	int						getCurrentFrame(){ return m_currentFrame; }

	int						getSizeOfFrameList(){ return m_frameList.size(); }

	vector<Vector2>			getFramePosition(){ return m_framePosition; }
	void					setFramePosition(vector<Vector2> x){ m_framePosition = x; }

	void					setMakeEffect(bool x){ m_makeEffect = x; }
	bool					getMakeEffect(){ return m_makeEffect; }


	void					updateVelocity();
	void					update();
	void					draw(LPD3DXSPRITE SpriteHandler);

	//ItemsType				getType(){ return m_type; }

	void					setIsBreak(bool x){ m_isBreak = x; }
	bool					isBreak(){ return m_isBreak; }

	int						m_time; // thời gian để tính vận tốc ném xiên
};

