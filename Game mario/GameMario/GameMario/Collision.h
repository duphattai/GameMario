#pragma once
#include "SwapAABB.h"
#include "Object.h"

class Object;

class Collision
{
	float timeCollision;
	float veclocity_vx;
	float veclocity_vy;
	Box m_Character, m_Object;
public:
	Collision();

	DIR isCollision(Object *object, Box box);
	void init(Box _Animation, Box _Object);
};
