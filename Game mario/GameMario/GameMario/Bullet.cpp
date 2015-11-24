#include "Bullet.h"
#include "ReSource.h"
#include "BulletOwnedState.h"

Bullet::Bullet()
{
	m_sprite = ReSource::getInstance()->getSprite(IDImage::IMG_ITEMSHEET);

	// hard code, xác định animation cho bullet
	// <move>
	m_frameList.push_back(Frame(96, 144, 8, 8));
	m_frameList.push_back(Frame(104, 144, 8, 8));
	m_frameList.push_back(Frame(96, 152, 8, 8));
	m_frameList.push_back(Frame(104, 152, 8, 8));
	// </move>
	// <explode>
	m_frameList.push_back(Frame(112, 144, 16, 16));
	m_frameList.push_back(Frame(112, 160, 16, 16));
	m_frameList.push_back(Frame(112, 176, 16, 16));
	// </explode>

	m_stateMachine = new StateMachine<Bullet>(this);
	m_stateMachine->changeState(BulletIdle::getInstance());

	time = 0;
	m_isExplode = false;
	m_currentFrame = 0;

	m_checkCollision = new Collision();
}


Bullet::~Bullet()
{
	delete m_checkCollision;
}


void Bullet::updateVelocity()
{
	if (m_isActive)
		m_stateMachine->update();
}

void Bullet::update()
{
	if (!m_isActive) return;

	m_position.x += m_velocity.x;
	m_position.y += m_velocity.y;
}

void Bullet::draw(LPD3DXSPRITE SpriteHandler)
{
	m_sprite->setRect(m_frameList[m_currentFrame].rect);
	GameObject::draw(SpriteHandler);
}

bool Bullet::isCollision(GameObject* gameObject)
{
	if (m_stateMachine->isInState(*BulletExplode::getInstance()))
		return false;

	DIR dir = m_checkCollision->isCollision(this, gameObject);
	if (dir != DIR::NONE) // xảy ra va chạm
	{
		m_velocity = m_checkCollision->getVelocity();
		if (gameObject->getTypeObject() == TypeObject::Moving_Enemy)
		{
			m_isExplode = true;
		}
		else
		{
			if (dir == DIR::TOP)
			{
				alpha = 3.14 / 6;
				time = 0;
			}
			else
				m_isExplode = true;
		}

		return true;
	}

	return false;
}