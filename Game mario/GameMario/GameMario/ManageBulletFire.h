#pragma once
#include "BulletFire.h"
class ManageBulletFire
{
	std::vector<BulletFire*>		m_gun;

	// give bullet is moving
	std::vector<BulletFire*>		m_giveBullet;
public:
	ManageBulletFire(int countBullet);
	~ManageBulletFire();


	void						shoot(int x, int y, float vpx, float vpy, SpriteEffect flip);

	std::vector<BulletFire*>	getBulletShooted(){ return m_giveBullet; }

	void						updateVelocity();
	void						update();

	bool						isCollision(GameObject*);

	void						draw(LPD3DXSPRITE SpriteHandler);
};

