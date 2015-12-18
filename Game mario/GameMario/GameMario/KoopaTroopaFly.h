#pragma once
#include "Enemy.h"

class KoopaTroopaFly : public Enemy
{
public:
	float						time;
	float						alpha;

	bool					    isCollision(GameObject*);

	KoopaTroopaFly();
	~KoopaTroopaFly();
};

