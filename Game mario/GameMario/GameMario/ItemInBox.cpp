﻿#include "ItemInBox.h"
#include "ReSource.h"
#include "ItemOwnedState.h"



// hard code, thiết lập frameList cho từng loại item
vector<Frame> getFrameAnimationBy(ItemsType type)
{
	vector<Frame> frameItem;
	RECT rect;
	switch (type)
	{
	case IT_COIN:
		for (int i = 0; i < 4; i++)
		{
			// frame i
			rect.left = 0 + 16 * i;
			rect.right = rect.left + 16;
			rect.top = 112;
			rect.bottom = rect.top + 16;

			frameItem.push_back(Frame(0, rect));
		}
		break;
	case IT_MUSHROOM_BIGGER:
		// (0,0,16,16)
		rect.left = 0;
		rect.right = rect.left + 16;
		rect.top = 0;
		rect.bottom = rect.top + 16;

		frameItem.push_back(Frame(0, rect));
		break;
	case IT_MUSHROOM_UP:
		// (16,0,16,16)
		rect.left = 16;
		rect.right = rect.left + 16;
		rect.top = 0;
		rect.bottom = rect.top + 16;

		frameItem.push_back(Frame(0, rect));
		break;
	case IT_GUN:
		for (int i = 0; i < 4; i++)
		{
			// frame i
			rect.left = 0 + 16 * i;
			rect.right = rect.left + 16;
			rect.top = 32;
			rect.bottom = rect.top + 16;

			frameItem.push_back(Frame(0, rect));
		}
		break;
	case IT_STAR:
		for (int i = 0; i < 4; i++)
		{
			// frame i
			rect.left = 0 + 16 * i;
			rect.right = rect.left + 16;
			rect.top = 48;
			rect.bottom = rect.top + 16;

			frameItem.push_back(Frame(0, rect));
		}
		break;
	case IT_BRICK:
		break;
	}

	return frameItem;
}

ItemInBox::ItemInBox(ItemsType type)
{
	m_isActive = false; // default
	m_stateMachine = new StateMachine<ItemInBox>(this);
	m_sprite = (ReSource::getInstance()->getSprite(IDImage::IMG_ITEMSHEET));
	m_checkCollision = new Collision();
	setItemsType(type);
}


ItemInBox::~ItemInBox()
{
}

void ItemInBox::setItemsType(ItemsType type)
{
	if (type == ItemsType::IT_STAR)
		m_mathematical = &vanTocNemXien;

	m_currentFrame = 0;
	m_type = type;
	m_frameList = getFrameAnimationBy(m_type);
	m_stateMachine->changeState(ItemInLuckyBoxIdle::getInstance());
}

void ItemInBox::updateVelocity()
{
	if (m_stateMachine != nullptr && m_isActive)
		m_stateMachine->update();
}

void ItemInBox::update()
{
	if (m_isActive)
	{
		m_position.x += m_velocity.x;
		m_position.y += m_velocity.y;

		// xứ lý va chạm đối với item star
		if (m_type == ItemsType::IT_STAR)
		{
			if (m_dirCollision != DIR::NONE)
			{
				m_time = 0;
				if (m_dirCollision == DIR::TOP)
				{
					StarItem::getInstance()->alpha = 3.14 / 3;
					StarItem::getInstance()->v = 6;
				}
				else
				{
					StarItem::getInstance()->alpha = 0;
					StarItem::getInstance()->v = 6;
					// dựa vào flip để xác định hướng vận tốc
					if (m_dirCollision == DIR::LEFT)
						m_flip = SpriteEffect::Flip;
					else 
						m_flip = SpriteEffect::None;
				}
					
			}
		}
	}
}

Box ItemInBox::getBouding()
{
	GameObject::getBouding();

	m_box.x += 4;
	m_box.width = 8;

	return m_box;
}

bool ItemInBox::isCollision(GameObject* gameObject)
{
	// xét va chạm với stand position và item
	// nếu item không active thì không xét
	// velocity.y > 0 đang thực hiện animation 
	if (!m_isActive || m_stateMachine->isInState(*ItemInLuckyBoxIdle::getInstance()))
		return false;

	DIR dir = m_checkCollision->isCollision(this, gameObject);
	if (dir != DIR::NONE)
	{
		m_velocity = m_checkCollision->getVelocity();
		
		if (dir == DIR::LEFT)
			m_flip = SpriteEffect::Flip;
		else if (dir == DIR::RIGHT)
			m_flip = SpriteEffect::None;
		

		if (m_dirCollision == DIR::NONE)
			m_dirCollision = dir;
		return true;
	}

	return false;
}


void ItemInBox::draw(LPD3DXSPRITE SpriteHandler)
{	
	m_sprite->setRect(m_frameList[m_currentFrame].rect);
	GameObject::draw(SpriteHandler);

	m_dirCollision = DIR::NONE;
}

