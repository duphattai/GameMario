#include "FloatingBar.h"
#include "ReSource.h"
#include "Mario.h"
FloatingBar::FloatingBar(FloatingBarMove move)
{
	m_sprite = ReSource::getInstance()->getSprite(IDImage::IMG_ITEMSHEET);
	m_typeMove = move;
	m_width = 48;
	m_height = 7;
	m_typeObject = Dynamic_Item;

	m_frameList.push_back(Frame(64, 129, 16, 8));
	m_currentFrame = 0;

	if (move == FloatingBarMove::MoveUp)
		m_velocity = Vector2(0, 2);
	else if(move == FloatingBarMove::MoveDown)
		m_velocity = Vector2(0, -1.0f);
	else if (move == FloatingBarMove::MoveLeft || move == FloatingBarMove::MoveRight)
		m_velocity = Vector2(2, 0);
}
FloatingBar::~FloatingBar()
{
}

void FloatingBar::update()
{
	m_position.x += m_velocity.x;
	m_position.y += m_velocity.y;

	if (m_typeMove == FloatingBarMove::MoveUp)
	{
		if (m_position.y > SCREEN_HEIGHT)
			m_position.y = 0;
	}
	else if (m_typeMove == FloatingBarMove::MoveDown)
	{
		if (m_position.y < 0)
			m_position.y = SCREEN_HEIGHT;
	}
}
void FloatingBar::updateVelocity()
{
	if (!m_isActive) return;
}


bool FloatingBar::isCollision(GameObject* gameObject)
{
	Mario* mario = dynamic_cast<Mario*>(gameObject);
	if (mario != nullptr)
	{
		// nếu mario chết thì không xét va chạm
		if (mario->isDead()) 
			return false;

		DIR dir = Collision::getInstance()->isCollision(gameObject, this);
		if (dir != DIR::NONE)
		{
			if (mario->getDirCollision() == DIR::NONE)
				mario->setDirCollision(dir);

			Vector2 gameObjectVelocity = Collision::getInstance()->getVelocity();
			if (dir == DIR::TOP)
			{
				//// thiết lập vận tốc
				switch (m_typeMove)
				{
				case FloatingBarMove::MoveUp:
					gameObjectVelocity.y += m_velocity.y;
					break;
				case FloatingBarMove::MoveLeft: case FloatingBarMove::MoveRight:
					gameObjectVelocity.x += m_velocity.x;
					break;
				}

				if (mario->getFSM() == FSM_Mario::FALL || mario->getFSM() == FSM_Mario::RUN) // fall gặp vật cản
					mario->setLocation(Location::LOC_ON_GROUND);
			}
			mario->setVelocity(gameObjectVelocity);
		}
	}

	return false;
}

void FloatingBar::draw(LPD3DXSPRITE SpriteHandler)
{
	if (!m_isActive) return;

	m_sprite->setRect(m_frameList[m_currentFrame].rect);
	for (int i = 0; i < m_width / 16; i++)
		m_sprite->draw(SpriteHandler, D3DXVECTOR2(m_position.x + 16 / 2 + i * 16, m_position.y + 8 / 2), D3DXVECTOR2(1.0f, 1.0f), m_worldPosition.x, m_worldPosition.y);
}

Box FloatingBar::getBouding()
{
	return GameObject::getBouding();
}
