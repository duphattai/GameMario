#include "Object.h"

Object::Object()
{
	m_IdImage = -1;

	m_Flip = SpriteEffect::None;
	m_Scale = D3DXVECTOR2(1.0f, 1.0f);
	//m_Draw = true;

	m_WorldPosition.x = m_WorldPosition.y = 0;
	m_Velocity = Vector2(0, 0);
	m_alpha = D3DCOLOR_XRGB(255, 255, 255);
}
Object::~Object()
{
	delete m_Gametime;
	m_IdImage = NULL;
}

void Object::setAlphaColor(D3DXCOLOR alpha)
{
	m_alpha = alpha;
}
D3DXCOLOR Object::getAlphaColor()
{
	return m_alpha;
}

void Object::setWorldPosition(Vector2 vector)
{
	m_WorldPosition = vector;
}
Vector2 Object::getWorldPosition()
{
	return m_WorldPosition;
}


void Object::setPosition(float x, float y)
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


int Object::getHeight()
{
	return m_Sprite->getHeight();
}
int Object::getWidth()
{
	return m_Sprite->getWidth();
}


void Object::setLocation(int location)
{
	m_location = (Location)location;
}
Location Object::getLocation()
{
	return m_location;
}

SpriteEffect Object::getFliping()
{
	return m_Flip;
}
void Object::setFliping(SpriteEffect flip)
{
	m_Flip = flip;
}


void Object::setDirCollision(DIR dir)
{
	m_DirCollision = dir;
}
DIR Object::getDirCollision()
{
	return m_DirCollision;
}


void Object::draw(LPD3DXSPRITE SpriteHandler)
{
	if (m_Flip == SpriteEffect::Flip)
		m_Scale = D3DXVECTOR2(-1.0f * SCALE, 1.0f * SCALE);
	else
		m_Scale = D3DXVECTOR2(1.0f * SCALE, 1.0f * SCALE);

	m_Sprite->draw(SpriteHandler, D3DXVECTOR2(m_Position.x + m_Sprite->getWidth() * SCALE / 2, m_Position.y + m_Sprite->getHeight() * SCALE / 2), m_Scale, m_WorldPosition.x, m_WorldPosition.y, m_alpha);
}

