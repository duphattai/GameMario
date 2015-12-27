#pragma once
#include "Hammer.h"

class ManageHammer
{
	std::vector<Hammer*>		m_gun;

	// give bullet is moving
	std::vector<Hammer*>		m_giveBullet;
public:
	ManageHammer(int countBullet);
	~ManageHammer();

	void						shoot(int x, int y, float vpx, float vpy, SpriteEffect flip);

	std::vector<Hammer*>		getBulletShooted(){ return m_giveBullet; }

	void						updateVelocity();
	void						update();

	bool						isCollision(GameObject*);

	void						draw(LPD3DXSPRITE SpriteHandler);

};
