#pragma once
#include "Enemy.h"

class Goomba : public Enemy
{
public:
	Goomba();
	~Goomba();

	bool					isCollision(GameObject*);
};

