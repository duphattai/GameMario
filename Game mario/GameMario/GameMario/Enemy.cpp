#include "Enemy.h"
#include "ReSource.h"
#include "KoopaTroopaOwnedState.h"
#include "GoombaOwnedState.h"
#include "KeyBoard.h"
#include "Mario.h"
#include "MarioOwnedState.h"
#include "Global.h"
#include "SoundClass.h"


extern LPD3DXFONT m_font;

Enemy::Enemy()
{
	m_beAttack = BeAttack::None;
	m_sprite = ReSource::getInstance()->getSprite(IDImage::IMG_ENEMYSHEET);
	m_typeObject = TypeObject::Moving_Enemy;
	m_flip = SpriteEffect::None;
}
Enemy::~Enemy()
{
}


void Enemy::updateVelocity()
{
	m_stateMachine->update();
}

void Enemy::update()
{
	m_position.x += m_velocity.x;
	m_position.y += m_velocity.y;
}

void Enemy::draw(LPD3DXSPRITE SpriteHandler)
{
	m_sprite->setRect(m_frameList[m_currentFrame].rect);
	if (m_isActive)
	{
		if (m_flip == SpriteEffect::Flip)
			m_scale = D3DXVECTOR2(-1.0f, 1.0f);
		else
			m_scale = D3DXVECTOR2(1.0f, 1.0f);

		if (m_beAttack == BeAttack::DeathByGun)
			m_scale.y = -1.0f;

		m_sprite->draw(SpriteHandler, D3DXVECTOR2(m_position.x + m_sprite->getWidth() / 2, m_position.y + m_sprite->getHeight() / 2), m_scale, m_worldPosition.x, m_worldPosition.y, m_alpha);

		if (m_text.size() != 0)
		{
			m_positionText.y += 2;
			RECT rct;
			rct.left = m_positionText.x - m_worldPosition.x;
			rct.right = rct.left + 20;
			rct.top = VIEW_PORT_Y - (m_positionText.y + m_sprite->getHeight());
			rct.bottom = rct.top + 12;

			m_font->DrawText(NULL, m_text.c_str(), -1, &rct, 0, D3DCOLOR_XRGB(255, 255, 255));
		}
	}
}


void Enemy::setCurrentFrame(int frame)
{
	m_currentFrame = frame;
	m_width = abs(m_frameList[m_currentFrame].rect.right - m_frameList[m_currentFrame].rect.left) - 2;
	m_height = abs(m_frameList[m_currentFrame].rect.bottom - m_frameList[m_currentFrame].rect.top) - 2;
}

Box Enemy::getBouding()
{
	m_box = GameObject::getBouding();
	return m_box;
}
