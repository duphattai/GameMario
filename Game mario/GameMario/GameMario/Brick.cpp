#include "Brick.h"
#include "ReSource.h"
#include "ItemOwnedState.h"

Brick::Brick(Vector2 position, IDMap idMap)
{
	m_position.x = position.x;
	m_position.y = position.y;
	m_width = 16;
	m_height = 16;


	// hard code
	// tọa độ mini brick trong items.png
	for (int i = 0; i < 2; i++)
		m_frameList.push_back(Frame(68 + (int)idMap % 2 * 144, 20 + 16 * i, 8, 8));


	m_framePosition.push_back(Vector2(position.x, position.y));
	m_framePosition.push_back(Vector2(position.x + 8, position.y));
	m_framePosition.push_back(Vector2(position.x, position.y + 8));
	m_framePosition.push_back(Vector2(position.x + 8, position.y + 8));


	m_isBreak = false;
	m_velocity = Vector2(0, 0);
	m_timeAnimation = 3;
	m_currentFrame = 0;
	m_stateMachine = new StateMachine<Brick>(this);
	m_stateMachine->changeState(BrickIdle::getInstance());

	m_index = 2 + 66 * (int)idMap % 2;
}
Brick::~Brick()
{
	delete m_stateMachine;
}

void Brick::update()
{
	m_position.x += m_velocity.x;
	m_position.y += m_velocity.y;
}
void Brick::updateVelocity()
{
	if (m_isActive)
		m_stateMachine->update();
}


void Brick::draw(LPD3DXSPRITE SpriteHandler)
{
	if (!m_isActive) return;

	if (!m_isBreak)
	{
		m_sprite = ReSource::getInstance()->getSprite(IDImage::IMG_TILEMAP);
		m_sprite->setIndex(m_index);
		GameObject::draw(SpriteHandler);
	}
	else
	{
		m_sprite = ReSource::getInstance()->getSprite(IDImage::IMG_ITEMSHEET);
		for each (Vector2 item in m_framePosition)
		{
			m_sprite->setRect(m_frameList[m_currentFrame].rect);
			m_position.x = item.x;
			m_position.y = item.y;

			GameObject::draw(SpriteHandler);
		}
	}
}