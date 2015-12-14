#include "Gun.h"
#include "SoundClass.h"
#include "Camera.h"

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
		(*temp)->shoot(flip, Vector2(x, y + 22), Vector2(vpx, vpy));
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
	// Cập nhật vận tốc
	for (std::vector<Bullet*>::iterator temp = m_giveBullet.begin(); temp != m_giveBullet.end(); temp++)
		(*temp)->updateVelocity();


	// Cập nhật kho đạn: lấy những viên đạn trạng thái active = false vào kho đạn
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


	// Hủy đạn khi ra khỏi màn hình
	for each (Bullet* bullet in getBulletShooted())
	{
		if (bullet->isActive() && bullet->getPosition().x < Camera::getInstance()->getViewport().x
			|| bullet->getPosition().x > Camera::getInstance()->getViewport().x + SCREEN_WIDTH)
			bullet->setTimeToLive(0);
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