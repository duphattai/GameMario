#include "GameObject.h"

GameObject::GameObject()
{
	m_Flip = SpriteEffect::None;
	m_Scale = D3DXVECTOR2(1.0f, 1.0f);
	//m_Draw = true;

	m_WorldPosition.x = m_WorldPosition.y = 0;
	m_Velocity = Vector2(0, 0);
	m_alpha = D3DCOLOR_XRGB(255, 255, 255);
}
GameObject::~GameObject()
{
}

void GameObject::setAlphaColor(D3DXCOLOR alpha)
{
	m_alpha = alpha;
}
D3DXCOLOR GameObject::getAlphaColor()
{
	return m_alpha;
}

void GameObject::setWorldPosition(Vector2 vector)
{
	m_WorldPosition = vector;
}
Vector2 GameObject::getWorldPosition()
{
	return m_WorldPosition;
}


void GameObject::setPosition(float x, float y)
{
	m_Position.x = x;
	m_Position.y = y;
}
Vector2 GameObject::getPosition()
{
	return Vector2(m_Position.x, m_Position.y);
}


void GameObject::setVelocity(Vector2 velocity)
{
	m_Velocity = velocity;
}
Vector2 GameObject::getVelocity()
{
	return m_Velocity;
}


int GameObject::getHeight()
{
	return m_Sprite->getHeight();
}
int GameObject::getWidth()
{
	return m_Sprite->getWidth();
}


void GameObject::setLocation(int location)
{
	m_location = (Location)location;
}
Location GameObject::getLocation()
{
	return m_location;
}

SpriteEffect GameObject::getFliping()
{
	return m_Flip;
}
void GameObject::setFliping(SpriteEffect flip)
{
	m_Flip = flip;
}


void GameObject::setDirCollision(DIR dir)
{
	m_DirCollision = dir;
}
DIR GameObject::getDirCollision()
{
	return m_DirCollision;
}

void GameObject::setSpriteSheet(Sprite* sprite)
{
	m_Sprite = sprite;
}

void GameObject::setIndexSprite(int index)
{
	m_index = index;
	m_Sprite->setIndex(index);
}
int GameObject::getIndexSprite()
{
	return m_index;
}

Box GameObject::getBouding()
{
	if (m_Sprite != nullptr)
		m_Box = Box(m_Position.x, m_Position.y, m_Sprite->getWidth(), m_Sprite->getHeight(), m_Velocity.x, m_Velocity.y);
	
	return m_Box;
}

void GameObject::setBox(Box box)
{
	m_Box = box;
}
Box GameObject::getBox()
{
	return m_Box;
}

void GameObject::setTypeObject(TypeObject type)
{
	m_typeObject = type;
}
TypeObject GameObject::getTypeObject()
{
	return m_typeObject;
}


void GameObject::draw(LPD3DXSPRITE SpriteHandler)
{
	if (m_Flip == SpriteEffect::Flip)
		m_Scale = D3DXVECTOR2(-1.0f, 1.0f);
	else
		m_Scale = D3DXVECTOR2(1.0f, 1.0f);

	m_Sprite->draw(SpriteHandler, D3DXVECTOR2(m_Position.x + m_Sprite->getWidth() / 2, m_Position.y + m_Sprite->getHeight() / 2), m_Scale, m_WorldPosition.x, m_WorldPosition.y, m_alpha);
}

