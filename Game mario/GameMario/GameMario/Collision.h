#pragma once
#include "SwapAABB.h"
#include "GameObject.h"

class GameObject;

class Collision
{
	float veclocity_vx;
	float veclocity_vy;
public:
	Collision();
	DIR isCollision(GameObject*, GameObject*);
	Vector2	getVelocity();
};
