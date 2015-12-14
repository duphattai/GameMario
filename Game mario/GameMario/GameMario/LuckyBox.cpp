#include "LuckyBox.h"
#include "ReSource.h"
#include "ItemOwnedState.h"
#include "Mario.h"

LuckyBox::LuckyBox(LuckyBoxsType type, ItemTypes itemType, IDMap idMap, int countCoin)
{
	m_countItem = countCoin;
	m_itemType = itemType;

	// hard code, tọa độ của animation luckybox trong items.png
	if (itemType == ItemTypes::YellowLuckyBox)
	{
		for (int i = 0; i < 4; i++)
			m_frameList.push_back(Frame(16 * i + ((int)idMap % 2) * 144, 80, 16, 16));
	}
	else if (itemType == ItemTypes::BrickLuckyBox)
	{
		m_frameList.push_back(Frame(32, 32 * (int)idMap, 16, 16));
	}
	// tọa độ của luckybox (không còn item) trong tileset.png
	m_frameList.push_back(Frame(48, 32 * (int)idMap % 2, 16, 16));

	//end
	m_item = new ItemInBox(type);
	m_item->setTypeObject(Moving_Item);

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
}

void LuckyBox::updateVelocity()
{
	m_stateMachine->update();
}

void LuckyBox::update()
{
	m_position.x += m_velocity.x;
	m_position.y += m_velocity.y;
}

bool LuckyBox::isCollision(GameObject* gameObject)
{
	// mario xét va chạm với luckybox
	Mario* mario = dynamic_cast<Mario*>(gameObject);
	if (mario != nullptr)
	{
		DIR dir = Collision::getInstance()->isCollision(mario, this);
		if (dir != DIR::NONE) // xảy ra va chạm
		{
			if (mario->getDirCollision() == DIR::NONE)
				mario->setDirCollision(dir);

			mario->setVelocity(Collision::getInstance()->getVelocity());
			if (dir == DIR::BOTTOM)
			{
				setMakeEffect(true);
			}
			else if (dir == DIR::TOP)
			{
				if (mario->getFSM() == FSM_Mario::FALL || mario->getFSM() == FSM_Mario::RUN) // fall gặp vật cản
					mario->setLocation(Location::LOC_ON_GROUND);
			}
		}

	}

	return false;
}

void LuckyBox::draw(LPD3DXSPRITE SpriteHandler)
{
	m_sprite->setRect(m_frameList[m_currentFrame].rect);
	GameObject::draw(SpriteHandler);
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