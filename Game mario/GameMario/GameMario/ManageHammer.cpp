#include "ManageHammer.h"
#include "Camera.h"
#include "HammerOwnedState.h"
ManageHammer::ManageHammer(int countBullet)
{
	for (int i = 0; i < countBullet; i++)
	{
		Hammer* temp = new Hammer();
		m_gun.push_back(temp);
	}
}


void ManageHammer::shoot(int x, int y, float vpx, float vpy, SpriteEffect flip)
{
	for (std::vector<Hammer*>::iterator temp = m_gun.begin(); temp != m_gun.end(); temp++)
	{
		(*temp)->shoot(flip, Vector2(x, y), Vector2(vpx, vpy));
		m_giveBullet.push_back(*temp);

		m_gun.erase(temp);
		break;
	}
}

ManageHammer::~ManageHammer()
{
	for (std::vector<Hammer*>::iterator temp = m_gun.begin(); temp != m_gun.end(); temp++)
	{
		Hammer* x = *temp;

		temp = m_gun.erase(temp);
		if (m_gun.size() == 0)
			break;
		if (temp != m_gun.begin()) temp--;
	}

	for (std::vector<Hammer*>::iterator temp = m_giveBullet.begin(); temp != m_giveBullet.end(); temp++)
	{
		Hammer* x = *temp;

		temp = m_giveBullet.erase(temp);
		if (m_giveBullet.size() == 0)
			break;
		if (temp != m_giveBullet.begin()) temp--;
	}

	// Hủy vùng nhớ
	delete HammerExplode::getInstance();
	delete HammerIdle::getInstance();
	delete HammerMoving::getInstance();
}


void ManageHammer::updateVelocity()
{
	// Cập nhật vận tốc
	for (std::vector<Hammer*>::iterator temp = m_giveBullet.begin(); temp != m_giveBullet.end(); temp++)
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

void ManageHammer::update()
{
	for (std::vector<Hammer*>::iterator temp = m_giveBullet.begin(); temp != m_giveBullet.end(); temp++)
	{
		(*temp)->update();
	}


	// Hủy đạn khi ra khỏi màn hình
	for each (Hammer* bullet in getBulletShooted())
	{
		if (bullet->isActive() && bullet->getPosition().x < Camera::getInstance()->getViewport().x
			|| bullet->getPosition().x > Camera::getInstance()->getViewport().x + SCREEN_WIDTH)
			bullet->setTimeToLive(0);
	}
}

void ManageHammer::draw(LPD3DXSPRITE SpriteHandler)
{
	for (std::vector<Hammer*>::iterator temp = m_giveBullet.begin(); temp != m_giveBullet.end(); temp++)
	{
		(*temp)->draw(SpriteHandler);
	}
}

bool ManageHammer::isCollision(GameObject* gameObject)
{
	// chi xét va cham với enemy, stand position, item
	if (gameObject->getTypeObject() == TypeObject::Dynamic_TiledMap || !gameObject->isActive())
		return false;

	for (std::vector<Hammer*>::iterator temp = m_giveBullet.begin(); temp != m_giveBullet.end(); temp++)
		(*temp)->isCollision(gameObject);

	return false;
}