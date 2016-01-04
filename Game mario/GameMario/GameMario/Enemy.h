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
	void					setCurrentFrame(int frame);//set frame hiện tại
	int						getCurrentFrame(){ return m_currentFrame; }//lấy frame hiện tại

	void					update();//update va chạm
	void					updateVelocity();//update vận tốc
	virtual void			draw(LPD3DXSPRITE SpriteHandler);

	virtual Box				getBouding();//lấy khung hình bao quanh	để xét collision	

	int						getSizeFrameList(){ return m_frameList.size(); }//lấy số lượng frame
	void					setFrameList(vector<Frame> list){ m_frameList = list; }//thiết lập frame

	BeAttack				getAttack(){ return m_beAttack; }
	void					setAttack(BeAttack beAttack){ m_beAttack = beAttack; }//lấy trạng thái bị mario tấn công chạm

	void					setPositionText(Vector2 position){ m_positionText = position; }//thiết lấp điểm text sẽ vẽ lên

	StateMachine<Enemy>*	getStateMachine(){ return m_stateMachine; }//lấy trạng thái của enemy

	Enemy();
	~Enemy();
};