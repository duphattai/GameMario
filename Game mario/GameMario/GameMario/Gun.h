#pragma once
#include "Bullet.h"

class Gun
{
	std::vector<Bullet*>		m_gun;

	// give bullet is moving
	std::vector<Bullet*>		m_giveBullet;
public:
	Gun(int countBullet);

	void						shoot(int x, int y, float vpx, float vpy, SpriteEffect flip);

	std::vector<Bullet*>		getBulletShooted(){ return m_giveBullet; }

	void						updateVelocity();
	void						update();

	bool						isCollision(GameObject*);

	void						draw(LPD3DXSPRITE SpriteHandler);
	~Gun();
};