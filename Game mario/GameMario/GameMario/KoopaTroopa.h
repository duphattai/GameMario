#pragma once
#include "Enemy.h"

class KoopaTroopa : public Enemy
{
public:
	KoopaTroopa();
	~KoopaTroopa();

	bool					isCollision(GameObject*);
};

