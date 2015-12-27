#pragma once
#include "Enemy.h"
#include "ManageHammer.h"
#include "ManageBulletFire.h"
class Bowser : public Enemy
{
	ManageHammer*			m_manageHammer;
	ManageBulletFire*		m_manageBulletFire;
public:
	Bowser();
	~Bowser();

	void					update();
	void					updateVelocity();
	void					draw(LPD3DXSPRITE SpriteHandler);

	ManageHammer*			getManageHammer(){ return m_manageHammer; }
	ManageBulletFire*		getManageBulletFire(){ return m_manageBulletFire; }

	bool					isCollision(GameObject*);
};

