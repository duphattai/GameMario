#include "Gun.h"
#include "SoundClass.h"

Gun::Gun(int countBullet)
{
	for (int i = 0; i < countBullet; i++)
	{
		Bullet* temp = new Bullet();
		m_gun.push_back(temp);
	}
}


void Gun::shoot(int x, int y, float vpx, float vpy, SpriteEffect flip)
{
	for (std::vector<Bullet*>::iterator temp = m_gun.begin(); temp != m_gun.end(); temp++)
	{
		(*temp)->setFliping(flip);
		(*temp)->setActive(true);
		(*temp)->setPosition(x, y + 22);
		(*temp)->setWorldPosition(Vector2(vpx, vpy));
		m_giveBullet.push_back(*temp);

		m_gun.erase(temp);
		break;
	}
}

Gun::~Gun()
{
	for (std::vector<Bullet*>::iterator temp = m_gun.begin(); temp != m_gun.end(); temp++)
	{
		Bullet* x = *temp;

		temp = m_gun.erase(temp);
		if (m_gun.size() == 0)
			break;
		if (temp != m_gun.begin()) temp--;
	}

	for (std::vector<Bullet*>::iterator temp = m_giveBullet.begin(); temp != m_giveBullet.end(); temp++)
	{
		Bullet* x = *temp;

		temp = m_giveBullet.erase(temp);
		if (m_giveBullet.size() == 0)
			break;
		if (temp != m_giveBullet.begin()) temp--;
	}
}


void Gun::updateVelocity()
{
	// cập nhật vận tốc
	for (std::vector<Bullet*>::iterator temp = m_giveBullet.begin(); temp != m_giveBullet.end(); temp++)
	{
		(*temp)->updateVelocity();
	}

	// cập nhật kho đạn
	for (int i = 0; i < m_giveBullet.size(); i++)
	{
		if (!m_giveBullet[i]->isActive())
		{
			m_gun.push_back(m_giveBullet[i]);
			m_giveBullet.erase(m_giveBullet.begin() + i);
			i--;
		}
	}
}

void Gun::update()
{
	for (std::vector<Bullet*>::iterator temp = m_giveBullet.begin(); temp != m_giveBullet.end(); temp++)
	{
		(*temp)->update();
	}
}

void Gun::draw(LPD3DXSPRITE SpriteHandler)
{
	for (std::vector<Bullet*>::iterator temp = m_giveBullet.begin(); temp != m_giveBullet.end(); temp++)
	{
		(*temp)->draw(SpriteHandler);
	}
}

bool Gun::isCollision(GameObject* gameObject)
{
	// chi xét va cham với enemy, stand position, item
	if (gameObject->getTypeObject() == TypeObject::Dynamic_TiledMap || !gameObject->isActive()) 
		return false;

	for (std::vector<Bullet*>::iterator temp = m_giveBullet.begin(); temp != m_giveBullet.end(); temp++)
		(*temp)->isCollision(gameObject);

	return false;
}