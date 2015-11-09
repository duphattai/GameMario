#include "LuckyBox.h"
#include "ReSource.h"
#include "ItemOwnedState.h"

LuckyBox::LuckyBox(ItemsType type, int countCoin)
{
	m_type = type;
	m_countItem = countCoin;

	m_currentFrame = 0;
	// hard code, tọa độ của luckybox trong ItemSheet
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
	m_sprite = ReSource::getInstance()->getSprite(IDImage::IMG_ITEMSHEET);

	m_stateMachine = new StateMachine<LuckyBox>(this);
	m_stateMachine->changeState(IdleLuckyBox::getInstance());
}

LuckyBox::~LuckyBox()
{
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

void LuckyBox::changeItemsType(ItemsType type)
{
	m_item->setItemsType(type);
}