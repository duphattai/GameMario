#pragma once
#include "SwapAABB.h"
#include "GameObject.h"

class GameObject;

class Collision
{
	float veclocity_vx;
	float veclocity_vy;

	static Collision* m_instance;
	Collision();
public:
	static Collision* getInstance();
	DIR isCollision(GameObject*, GameObject*);
	Vector2	getVelocity();
};
