#include "GameObject.h"
#include "Camera.h"
extern LPD3DXFONT m_font;

GameObject::GameObject()
{
	m_flip = SpriteEffect::None;
	m_scale = D3DXVECTOR2(1.0f, 1.0f);

	m_velocity = Vector2(0, 0);
	m_alpha = D3DCOLOR_XRGB(255, 255, 255);
	m_isActive = true;
	m_status = StatusObject::ALIVE;
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


void GameObject::setPosition(float x, float y)
{
	m_position.x = x;
	m_position.y = y;
}
Vector2 GameObject::getPosition()
{
	return Vector2(m_position.x, m_position.y);
}


void GameObject::setVelocity(Vector2 velocity)
{
	m_velocity = velocity;
}
Vector2 GameObject::getVelocity()
{
	return m_velocity;
}


int GameObject::getHeight()
{
	return m_height;
}
int GameObject::getWidth()
{
	return m_width;
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
	return m_flip;
}
void GameObject::setFliping(SpriteEffect flip)
{
	m_flip = flip;
}


void GameObject::setDirCollision(DIR dir)
{
	m_dirCollision = dir;
}
DIR GameObject::getDirCollision()
{
	return m_dirCollision;
}

void GameObject::setSpriteSheet(Sprite* sprite)
{
	m_sprite = sprite;
}

void GameObject::setIndexSprite(int index)
{
	m_index = index;
	m_sprite->setIndex(index);
	m_width = m_sprite->getWidth();
	m_height = m_sprite->getHeight();
}
int GameObject::getIndexSprite()
{
	return m_index;
}

Box GameObject::getBouding()
{
	if (m_sprite != nullptr)
		m_box = Box(m_position.x, m_position.y, m_width, m_height, m_velocity.x, m_velocity.y);
	
	return m_box;
}

void GameObject::setBox(Box box)
{
	m_box = box;
}
Box GameObject::getBox()
{
	return m_box;
}


void GameObject::setStatusObject(StatusObject status)
{
	m_status = status;
}
StatusObject GameObject::getStatusOBject()
{
	return m_status;
}

void GameObject::setActive(bool x)
{
	m_isActive = x;
}
bool GameObject::isActive()
{
	return m_isActive;
}


void GameObject::setWorldPosition(Vector2 viewPort)
{
	m_worldPosition = viewPort;
}

void GameObject::setTypeObject(TypeObject type)
{
	m_typeObject = type;
}
TypeObject GameObject::getTypeObject()
{
	return m_typeObject;
}

void GameObject::setText(wstring text)
{
	m_text = text;
}


void GameObject::draw(LPD3DXSPRITE SpriteHandler)
{
	if (m_isActive)
	{
		if (m_text.size() == 0)
		{
			if (m_flip == SpriteEffect::Flip)
				m_scale = D3DXVECTOR2(-1.0f, 1.0f);
			else
				m_scale = D3DXVECTOR2(1.0f, 1.0f);

			m_sprite->draw(SpriteHandler, D3DXVECTOR2(m_position.x + m_width / 2, m_position.y + m_height / 2), m_scale, m_worldPosition.x, m_worldPosition.y, m_alpha);
		}
		else
		{
			drawText(m_text, Vector2(m_position.x - m_worldPosition.x, VIEW_PORT_Y - (m_position.y + m_height)));
		}
	}
}

void GameObject::drawText(wstring text, Vector2 position)
{
	RECT rct;
	rct.left = position.x;
	rct.right = rct.left + 12 * text.length();
	rct.top = position.y;
	rct.bottom = position.y + 12;

	m_font->DrawText(NULL, text.c_str(), -1, &rct, 0, D3DCOLOR_XRGB(255, 255, 255));
}