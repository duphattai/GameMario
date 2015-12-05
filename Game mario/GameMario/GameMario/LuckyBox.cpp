#include "LuckyBox.h"
#include "ReSource.h"
#include "ItemOwnedState.h"

LuckyBox::LuckyBox(LuckyBoxsType type, ItemTypes itemType, IDMap idMap, int countCoin)
{
	m_countItem = countCoin;
	m_itemType = itemType;

	// hard code, tọa độ của animation luckybox trong items.png
	if (itemType == ItemTypes::YellowLuckyBox)
	{
		for (int i = 0; i < 4; i++)
			m_frameList.push_back(Frame(16 * i + (int)idMap * 144, 80, 16, 16));
	}
	else if (itemType == ItemTypes::BrickLuckyBox)
	{
		m_frameList.push_back(Frame(16, 32 * (int)idMap, 16, 16));
	}
	// tọa độ của luckybox (không còn item) trong tileset.png
	m_frameList.push_back(Frame(48, 32 * (int)idMap, 16, 16));

	//end
	m_item = new ItemInBox(type);

	m_stateMachine = new StateMachine<LuckyBox>(this);
	if (itemType == ItemTypes::BrickLuckyBox)
	{
		m_sprite = ReSource::getInstance()->getSprite(IDImage::IMG_TILEMAP);
		m_stateMachine->changeState(IdleBrickItem::getInstance());
	}
	else if (itemType == ItemTypes::YellowLuckyBox)
	{
		m_sprite = ReSource::getInstance()->getSprite(IDImage::IMG_ITEMSHEET);
		m_stateMachine->changeState(IdleLuckyBox::getInstance());
	}

	setCurrentFrame(0);
}

LuckyBox::~LuckyBox()
{
	delete m_stateMachine;
	delete m_item;
}

void LuckyBox::updateVelocity()
{
	m_stateMachine->update();
	m_item->updateVelocity();
}

void LuckyBox::update()
{
	m_position.x += m_velocity.x;
	m_position.y += m_velocity.y;

	m_item->update();
}

bool LuckyBox::isCollision(GameObject* gameObject)
{
	return m_item->isCollision(gameObject);
}

void LuckyBox::draw(LPD3DXSPRITE SpriteHandler)
{
	m_item->setWorldPosition(m_worldPosition);
	m_item->draw(SpriteHandler);

	m_sprite->setRect(m_frameList[m_currentFrame].rect);
	GameObject::draw(SpriteHandler);
}

void LuckyBox::changeItemsType(LuckyBoxsType type)
{	
	m_item->setItemsType(type);
}

Box LuckyBox::getBouding()
{
	m_box = GameObject::getBouding();

	return m_box;
}

void LuckyBox::setCurrentFrame(int frame)
{
	m_currentFrame = frame;
	m_width = abs(m_frameList[m_currentFrame].rect.right - m_frameList[m_currentFrame].rect.left);
	m_height = abs(m_frameList[m_currentFrame].rect.bottom - m_frameList[m_currentFrame].rect.top);
}