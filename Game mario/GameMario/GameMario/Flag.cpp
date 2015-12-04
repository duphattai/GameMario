#include "Flag.h"
#include "ReSource.h"
#include "Camera.h"

Flag::Flag(Vector2 position)
{
	// hard code
	m_width = 2;
	m_height = 128;

	m_makeEffect = false;
	m_finishAnimation = false;
	m_velocity = Vector2(0, 0);

	m_position.x = position.x - 8; // hard code
	m_position.y = position.y - m_height;
	m_flagPosition.x = position.x - 8;
	m_flagPosition.y = position.y;

	// thiết lập framelist 
	m_frameList.push_back(Frame(128, 32, 16, 16)); // tọa độ lá cờ
}
Flag::~Flag()
{
}

void Flag::updateVelocity()
{
	if (m_makeEffect)
		m_velocity = Vector2(0, -2);
}

void Flag::update()
{
	m_flagPosition.y += m_velocity.y;

	if (m_flagPosition.y < m_position.y)
	{
		m_flagPosition.y = m_position.y;
		m_finishAnimation = true;
		m_velocity.y = 0;
	}
}

void Flag::draw(LPD3DXSPRITE SpriteHandler)
{
	// vẽ lá cờ
	m_sprite = ReSource::getInstance()->getSprite(IDImage::IMG_ITEMSHEET);
	m_sprite->setRect(m_frameList[0].rect);
	m_sprite->draw(SpriteHandler, D3DXVECTOR2(m_flagPosition.x + 8, m_flagPosition.y + 8), D3DXVECTOR2(-1.0f, 1.0f), Camera::getInstance()->getViewport().x, Camera::getInstance()->getViewport().y);

	// vẽ cây cờ
	m_sprite = ReSource::getInstance()->getSprite(IDImage::IMG_TILEMAP);
	m_sprite->setIndex(313);
	for (int i = 0; i < m_height / 16; i++)
		m_sprite->draw(SpriteHandler, D3DXVECTOR2(m_position.x + 1, m_position.y + i * 16 + 8), D3DXVECTOR2(1.0f, 1.0f), Camera::getInstance()->getViewport().x, Camera::getInstance()->getViewport().y);
}

Box Flag::getBouding()
{
	m_box = Box(m_position.x, m_position.y, m_width, m_height);
	return m_box;
}