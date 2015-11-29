#include "LuckyBox.h"
#include "ReSource.h"
#include "ItemOwnedState.h"

LuckyBox::LuckyBox(LuckyBoxsType type, ItemTypes itemType, int countCoin)
{
	m_countItem = countCoin;
	m_itemType = itemType;

	// hard code, tọa độ của animation luckybox trong ItemSheet
	for (int i = 0; i < 4; i++)
	{
		RECT rect;
		// frame i
		rect.left = 0 + 16 * i;
		rect.right = rect.left + 16; 
		rect.top = 80; 
		rect.bottom = rect.top + 16;
		
		m_frameList.push_back(Frame(0, rect));
	}
	// tọa độ của sprite luckybox 2
	RECT rect;
	rect.left = 48;
	rect.right = 64;
	rect.top = 0;
	rect.bottom = 16;
	m_frameList.push_back(Frame(0, rect));

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

	if (m_itemType == ItemTypes::YellowLuckyBox)
		m_sprite->setRect(m_frameList[m_currentFrame].rect);
	else if (m_itemType == ItemTypes::BrickLuckyBox)
		m_sprite->setIndex(m_index);

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