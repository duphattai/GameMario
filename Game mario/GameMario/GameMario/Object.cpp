#include "Object.h"

Object::Object()
{
	m_IdImage = -1;

	m_Flip = SpriteEffect::None;
	m_Scale = D3DXVECTOR2(1.0f, 1.0f);
	//m_Draw = true;

	m_WorldPosition.x = m_WorldPosition.y = 0;
	m_Velocity = Vector2(0, 0);
}

void Object::setWorldPosition(Vector2 vector)
{
	m_WorldPosition = vector;
}

Vector2 Object::getWorldPosition()
{
	return m_WorldPosition;
}

void Object::setPosition(int x, int y)
{
	m_Position.x = x;
	m_Position.y = y;
}

Vector2 Object::getPosition()
{
	return Vector2(m_Position.x, m_Position.y);
}

void Object::setVelocity(Vector2 velocity)
{
	m_Velocity = velocity;
}

Vector2 Object::getVelocity()
{
	return m_Velocity;
}

Object::~Object()
{
	delete m_Gametime;
	m_IdImage = NULL;
}

void Object::draw(LPD3DXSPRITE SpriteHandler)
{
	if (m_Flip == SpriteEffect::Flip)
		m_Scale = D3DXVECTOR2(-1.0f, 1.0f);
	else
		m_Scale = D3DXVECTOR2(1.0f, 1.0f);

	m_Sprite->draw(SpriteHandler, D3DXVECTOR2(m_Position.x + m_Sprite->getWidth() / 2, m_Position.y + m_Sprite->getHeight() / 2), m_Scale, m_WorldPosition.x, m_WorldPosition.y);
}

