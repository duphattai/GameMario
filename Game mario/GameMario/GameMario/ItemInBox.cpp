#include "ItemInBox.h"
#include "ReSource.h"

ItemInBox::ItemInBox(std::vector<Frame> frameList, ItemsType type)
{
	m_frameList = frameList;
	m_currentFrame = 0;

	m_type = type;
	m_isCollision = false;
	m_stateMachine = new StateMachine<ItemInBox>(this);
	m_isActive = false;
	m_Velocity = Vector2(0, 5);
	setItemsType(type);
	m_Sprite = (ReSource::getInstance()->getSprite(IDImage::IMG_ITEMSHEET));
}


ItemInBox::~ItemInBox()
{
}

void ItemInBox::setItemsType(ItemsType type)
{
	m_type = type;

	switch (type)
	{
	case IT_COIN:
		m_currentFrame = ItemSheet::IS_COIN;
		//m_stateMachine->changeState(Coin::getInstance());
		break;
	case IT_MUSHROOM_BIGGER:
		m_currentFrame = ItemSheet::IS_MUSHROOM_BIGGER;
		//m_stateMachine->changeState(MushRoomBigger::getInstance());
		break;
	case IT_GUN:
		m_currentFrame = ItemSheet::IS_GUNFLOWER;
		//m_stateMachine->changeState(Gun::getInstance());
		break;
	case IT_STAR:
		m_currentFrame = ItemSheet::IS_STAR;
		break;
	case IT_MUSHROOM_UP:
		m_currentFrame = ItemSheet::IS_MUSHROOM_UP;
		break;
	case IT_BRICK:
		m_currentFrame = ItemSheet::IS_BREAKBRICK;
		//m_stateMachine->changeState(BreakBrick::getInstance());
		break;
	}
}

void ItemInBox::updateVelocity()
{
	if (m_stateMachine != nullptr && m_isActive)
		m_stateMachine->update();
}

void ItemInBox::update()
{
	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
}

void ItemInBox::draw(LPD3DXSPRITE SpriteHandler)
{
	if (m_isActive)
	{
		m_Sprite->setRect(m_frameList[m_currentFrame].rect);
		GameObject::draw(SpriteHandler);
	}
}

