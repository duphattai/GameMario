#include "Collision.h"

Collision::Collision()
{
}

DIR Collision::isCollision(Object *object, Box box)
{
	m_Object = box;
	m_Character = object->getBouding();
	veclocity_vx = object->getVelocity().x;
	veclocity_vy = object->getVelocity().y;
	float normalX = 0, normalY = 0;

	// vật nằm trong không gian của đối tượng 
	if (CheckAABB(m_Object, GetSweptBroadPhaseBox(m_Character)))
	{
		// kiem tra va cham không cần biết vận tốc cua doi tuong
		DIR dir = AABB(m_Character, m_Object);
		if (dir == DIR::NONE)
		{
			timeCollision = SweptAABB(m_Character, m_Object, normalX, normalY);

			if (timeCollision > 0.0f && timeCollision <= 1.0f)
			{
				// update velocity
				Vector2 velocity = object->getVelocity();
				if (abs(veclocity_vx) >= abs(m_Character.vx * timeCollision + normalX))
					velocity.x = m_Character.vx * timeCollision + normalX;
				if (abs(veclocity_vy) >= abs(m_Character.vy * timeCollision + normalY))
					velocity.y = m_Character.vy * timeCollision + normalY;
				
				object->setVelocity(velocity);

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
		else // xảy ra va chạm
		{
			Vector2 position = object->getPosition();
			Vector2 velocity = object->getVelocity();

			if (dir == TOP) // top
			{
				velocity.y = 0;
				position.y = m_Object.y + m_Object.height + 1;
			}
			else if (dir == DIR::BOTTOM) // bottom
			{
				velocity.y = 0;
				position.y = m_Object.y - m_Character.height - 1;
			}
			else if (dir == DIR::LEFT )  // left
			{
				velocity.x = 0;
				position.x = m_Object.x - m_Character.width - 1;
			}
			else if (dir == DIR::RIGHT)
			{
				velocity.x = 0;
				position.x = m_Object.x + m_Object.width + 1;
			}

			object->setPosition(position.x, position.y);
			object->setVelocity(velocity);

			return dir;
		}
	}
	else
	{
		return DIR::NONE;
	}
}
