#include "Coin.h"
#include "ReSource.h"

Coin::Coin()
{
	m_sprite = ReSource::getInstance()->getSprite(IDImage::IMG_ITEMSHEET);
	m_currentFrame = 0;

	// hard code, tọa độ sprite aniamtion của coin trong items.png
	m_frameList.push_back(Frame(3, 98, 10, 14));
	m_frameList.push_back(Frame(19, 98, 10, 14));
	m_frameList.push_back(Frame(35, 98, 10, 14));
	m_frameList.push_back(Frame(51, 98, 10, 14));

	m_timeAnimation = 5;
}


Coin::~Coin()
{
}

void Coin::update()
{
	if (m_status == StatusObject::DEAD && m_isActive)
	{
		setActive(false);
		PlaySound(L"Sounds/smb_coin.wav", NULL, SND_ASYNC);
	}
}
void Coin::updateVelocity()
{
	if (!m_isActive) return;

	if (--m_timeAnimation <= 0)
	{
		m_currentFrame++;
		if (m_currentFrame >= m_frameList.size())
			m_currentFrame = 0;

		m_timeAnimation = 5;
	}
}


void Coin::draw(LPD3DXSPRITE SpriteHandler)
{
	if (!m_isActive || m_status == StatusObject::DEAD) return;

	m_sprite->setRect(m_frameList[m_currentFrame].rect);
	GameObject::draw(SpriteHandler);
}