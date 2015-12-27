#include "Hammer.h"
#include "ReSource.h"
#include "Enemy.h"
#include "HammerOwnedState.h"
#include "Mario.h"

Hammer::Hammer()
{
	m_sprite = ReSource::getInstance()->getSprite(IDImage::IMG_ITEMSHEET);

	// hard code, xác định animation cho hammer trong item sheet
	// <move>
	m_frameList.push_back(Frame(64, 192, 16, 16));
	m_frameList.push_back(Frame(80, 192, 16, 16));
	m_frameList.push_back(Frame(80, 208, 16, 16));
	m_frameList.push_back(Frame(64, 208, 16, 16));
	// </move>
	// <explode>
	m_frameList.push_back(Frame(112, 144, 16, 16));
	m_frameList.push_back(Frame(112, 160, 16, 16));
	m_frameList.push_back(Frame(112, 176, 16, 16));
	// </explode>

	m_stateMachine = new StateMachine<Hammer>(this);
	m_stateMachine->changeState(HammerIdle::getInstance());

	time = 0;
	m_isExplode = false;
	m_currentFrame = 0;
}

Hammer::~Hammer()
{
	delete m_stateMachine;
}

void Hammer::updateVelocity()
{
	if (m_isActive)
		m_stateMachine->update();
}

void Hammer::update()
{
	if (!m_isActive) return;

	m_position.x += m_velocity.x;
	m_position.y += m_velocity.y;
}

void Hammer::draw(LPD3DXSPRITE SpriteHandler)
{
	m_sprite->setRect(m_frameList[m_currentFrame].rect);
	GameObject::draw(SpriteHandler);
}

bool Hammer::isCollision(GameObject* gameObject)
{
	if (m_stateMachine->isInState(*HammerExplode::getInstance()))
		return false;

	DIR dir = Collision::getInstance()->isCollision(this, gameObject);
	if (dir != DIR::NONE) // xảy ra va chạm
	{
		m_velocity = Collision::getInstance()->getVelocity();
		Mario* mario = dynamic_cast<Mario*>(gameObject);
		/*if (mario != nullptr)
			mario->setDead(true);*/

		m_isExplode = true;

		return true;
	}

	return false;
}

void Hammer::shoot(SpriteEffect flip, Vector2 position, Vector2 worldPosition)
{
	m_flip = flip;
	m_position.x = position.x;
	m_position.y = position.y;
	m_worldPosition = worldPosition;
	m_stateMachine->changeState(HammerIdle::getInstance());
	m_isActive = true;
}

void Hammer::setCurrentFrame(int frame)
{
	m_currentFrame = frame;
	m_width = abs(m_frameList[m_currentFrame].rect.left - m_frameList[m_currentFrame].rect.right);
	m_height = abs(m_frameList[m_currentFrame].rect.top - m_frameList[m_currentFrame].rect.bottom);
}
