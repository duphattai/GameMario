#pragma once
#include "Enemy.h"
class FireFlower : public Enemy
{

public:
	FireFlower();
	~FireFlower();

	int			m_timeMove;

	bool					isCollision(GameObject*);
};

