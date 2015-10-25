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


	
	// hard code, tọa độ của sprite trong itemsheet
	vector<Frame> frameItem;
	switch (m_type)
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
		m_item = new ItemInBox(frameItem, ItemsType::IT_COIN);
		break;
	case IT_MUSHROOM_BIGGER:
		// (0,0,16,16)
		rect.left = 0;
		rect.right = rect.left + 16;
		rect.top = 0;
		rect.bottom = rect.top + 16;

		frameItem.push_back(Frame(0, rect));
		m_item = new ItemInBox(frameItem, ItemsType::IT_MUSHROOM_BIGGER);
		break;
	case IT_MUSHROOM_UP:
		// (0,0,16,16)
		rect.left = 0;
		rect.right = rect.left + 16;
		rect.top = 16;
		rect.bottom = rect.top + 16;

		frameItem.push_back(Frame(0, rect));
		m_item = new ItemInBox(frameItem, ItemsType::IT_MUSHROOM_UP);

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
		m_item = new ItemInBox(frameItem, ItemsType::IT_GUN);
		m_item->getStateMachine()->changeState(Gun::getInstance());
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
		m_item = new ItemInBox(frameItem, ItemsType::IT_STAR);
		break;
	case IT_BRICK:
		break;
	}
	//end
	m_Sprite = ReSource::getInstance()->getSprite(IDImage::IMG_ITEMSHEET);

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
	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;

	m_item->update();
}

void LuckyBox::draw(LPD3DXSPRITE SpriteHandler)
{
	m_item->setWorldPosition(m_WorldPosition);
	m_item->draw(SpriteHandler);

	m_Sprite->setRect(m_frameList[m_currentFrame].rect);
	GameObject::draw(SpriteHandler);
}