#include "Collision.h"

Collision* Collision::m_instance = 0;

Collision* Collision::getInstance()
{
	if (m_instance == NULL)
		m_instance = new Collision();

	return m_instance;
}

Collision::Collision()
{
}

Vector2 Collision::getVelocity()
{
	return Vector2(veclocity_vx, veclocity_vy);
}

DIR Collision::isCollision(GameObject *moveObject, GameObject* dynamicObject)
{
	Box dynamicBox = dynamicObject->getBouding();
	Box moveBox = moveObject->getBouding();
	veclocity_vx = moveBox.vx;
	veclocity_vy = moveBox.vy;
	float normalX = 0, normalY = 0;

	// kiem tra va cham không cần biết vận tốc cua doi tuong
	DIR dir = AABB(moveBox, dynamicBox);
	if (dir == DIR::NONE)
	{
		// make dynamicBox not move
		moveBox.vx -= dynamicBox.vx;
		moveBox.vy -= dynamicBox.vy;
		dynamicBox.vx = dynamicBox.vy = 0;

		// vật nằm trong không gian của đối tượng 
		if (AABB(dynamicBox, GetSweptBroadPhaseBox(moveBox)) != DIR::NONE)
		{
			float timeCollision = SweptAABB(moveBox, dynamicBox, normalX, normalY);

			if (timeCollision > 0.0f && timeCollision <= 1.0f)
			{
				// update velocity
				if (abs(veclocity_vx) >= abs(moveBox.vx * timeCollision + normalX) && normalX != 0.0f)
					veclocity_vx = moveBox.vx * timeCollision + normalX;
				if (abs(veclocity_vy) >= abs(moveBox.vy * timeCollision + normalY) && normalY != 0.0f)
					veclocity_vy = moveBox.vy * timeCollision + normalY;


				if (normalX != 0.0f)
				{
					if (normalX == -1.0f)
						return DIR::LEFT;

					if (normalX == 1.0f)
						return DIR::RIGHT;
				}
				else
				{
					if (normalY != 0.0f)
					{
						if (normalY == 1.0f)
							return DIR::TOP;

						if (normalY == -1.0f)
							return DIR::BOTTOM;
					}
				}
			}
			else
			{
				return DIR::NONE;
			}
		}
		else
		{
			return DIR::NONE;
		}
	}
		
	else // xảy ra va chạm
	{
		if (dir == TOP) // top
		{
			moveBox.y = dynamicBox.y + dynamicBox.height + 1;
		}
		else if (dir == DIR::BOTTOM) // bottom
		{
			moveBox.y = dynamicBox.y - moveBox.height - 1;
		}
		else if (dir == DIR::LEFT)  // left
		{
			moveBox.x = dynamicBox.x - moveBox.width - 1;
		}
		else if (dir == DIR::RIGHT)
		{
			moveBox.x = dynamicBox.x + dynamicBox.width + 1;
		}

		moveObject->setPosition(moveBox.x, moveBox.y);

		return dir;
	}
}
