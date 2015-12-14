#include "Coin.h"
#include "ReSource.h"
#include "ScoreGame.h"
#include "Mario.h"

Coin::Coin(IDMap idMap)
{
	m_sprite = ReSource::getInstance()->getSprite(IDImage::IMG_ITEMSHEET);
	m_currentFrame = 0;

	// hard code, tọa độ sprite aniamtion của coin trong items.png
	m_frameList.push_back(Frame(3  + 144 * (int)idMap % 2, 98, 10, 14));
	m_frameList.push_back(Frame(19 + 144 * (int)idMap % 2, 98, 10, 14));
	m_frameList.push_back(Frame(35 + 144 * (int)idMap % 2, 98, 10, 14));
	m_frameList.push_back(Frame(51 + 144 * (int)idMap % 2, 98, 10, 14));

	m_timeAnimation = 5;
	m_width = 10;
	m_height = 14;
}


Coin::~Coin()
{
}

void Coin::update()
{
	if (m_status == StatusObject::DEAD && m_isActive)
	{
		ScoreGame::getInstance()->setCountOfCoin(ScoreGame::getInstance()->getCountOfCoin() + 1);
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

bool Coin::isCollision(GameObject* gameObject)
{
	if (!m_isActive || m_status == StatusObject::DEAD) 
		return false;

	Mario* mario = dynamic_cast<Mario*>(gameObject);
	if (mario != nullptr)
	{
		if (Collision::getInstance()->isCollision(this, mario) != DIR::NONE)
		{
			m_status = StatusObject::DEAD;
			return true;
		}
	}

	return false;
}

void Coin::draw(LPD3DXSPRITE SpriteHandler)
{
	if (!m_isActive || m_status == StatusObject::DEAD) return;

	m_sprite->setRect(m_frameList[m_currentFrame].rect);
	GameObject::draw(SpriteHandler);
}

Box Coin::getBouding()
{
	m_box = GameObject::getBouding();

	return m_box;
}