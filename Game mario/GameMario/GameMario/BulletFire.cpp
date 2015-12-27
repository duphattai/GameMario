#include "BulletFire.h"
#include "ReSource.h"
#include "Mario.h"
#include "BulletFireOwnedState.h"

BulletFire::BulletFire()
{
	m_sprite = ReSource::getInstance()->getSprite(IDImage::IMG_ITEMSHEET);

	// hard code, xác định animation cho hammer trong item sheet
	// <move>
	m_frameList.push_back(Frame(100, 128, 24, 8));
	m_frameList.push_back(Frame(100, 136, 24, 8));
	// </move>
	// <explode>
	m_frameList.push_back(Frame(112, 144, 16, 16));
	m_frameList.push_back(Frame(112, 160, 16, 16));
	m_frameList.push_back(Frame(112, 176, 16, 16));
	// </explode>

	m_stateMachine = new StateMachine<BulletFire>(this);
	m_stateMachine->changeState(BulletFireIdle::getInstance());

	m_isExplode = false;
	m_currentFrame = 0;
}

BulletFire::~BulletFire()
{
	delete m_stateMachine;
}

void BulletFire::updateVelocity()
{
	if (m_isActive)
		m_stateMachine->update();
}

void BulletFire::update()
{
	if (!m_isActive) return;

	m_position.x += m_velocity.x;
	m_position.y += m_velocity.y;
}

void BulletFire::draw(LPD3DXSPRITE SpriteHandler)
{
	m_sprite->setRect(m_frameList[m_currentFrame].rect);
	GameObject::draw(SpriteHandler);
}

bool BulletFire::isCollision(GameObject* gameObject)
{
	if (m_stateMachine->isInState(*BulletFireExplode::getInstance()))
		return false;

	DIR dir = Collision::getInstance()->isCollision(this, gameObject);
	if (dir != DIR::NONE) // xảy ra va chạm
	{
		m_velocity = Collision::getInstance()->getVelocity();
		Mario* mario = dynamic_cast<Mario*>(gameObject);
		if (mario != nullptr)
			mario->setDead(true);

		m_isExplode = true;
		return true;
	}

	return false;
}

void BulletFire::shoot(SpriteEffect flip, Vector2 position, Vector2 worldPosition)
{
	m_flip = flip;
	m_position.x = position.x;
	m_position.y = position.y;
	m_worldPosition = worldPosition;
	m_stateMachine->changeState(BulletFireIdle::getInstance());
	m_isActive = true;
}

void BulletFire::setCurrentFrame(int frame)
{
	m_currentFrame = frame;
	m_width = abs(m_frameList[m_currentFrame].rect.left - m_frameList[m_currentFrame].rect.right);
	m_height = abs(m_frameList[m_currentFrame].rect.top - m_frameList[m_currentFrame].rect.bottom);
}
