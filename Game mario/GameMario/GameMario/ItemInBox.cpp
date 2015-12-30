#include "ItemInBox.h"
#include "ReSource.h"
#include "ItemOwnedState.h"
#include "Mario.h"


// hard code, thiết lập frameList cho từng loại item
vector<Frame> getFrameAnimationBy(LuckyBoxsType type)
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
	}

	return frameItem;
}

ItemInBox::ItemInBox(LuckyBoxsType type)
{
	m_isActive = false; // default
	m_stateMachine = new StateMachine<ItemInBox>(this);
	m_sprite = (ReSource::getInstance()->getSprite(IDImage::IMG_ITEMSHEET));
	setItemType(type);
}

ItemInBox::~ItemInBox()
{
	delete m_stateMachine;
}

void ItemInBox::setItemType(LuckyBoxsType type)
{
	if (type == LuckyBoxsType::IT_STAR)
		m_mathematical = &vanTocNemXien;

	m_type = type;
	m_frameList = getFrameAnimationBy(m_type);
	setCurrentFrame(0);
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

		// Xứ lý va chạm đối với item star
		if (m_type == LuckyBoxsType::IT_STAR)
		{
			if (m_dirCollision != DIR::NONE)
			{
				//m_time = 0;
				if (m_dirCollision == DIR::TOP)
				{
					// Góc ném xiên
					StarItem::getInstance()->alpha = 3.14 / 3;
					// Vận tốc ban đầu
					StarItem::getInstance()->v = 6;
					m_time = 0;
				}
				else
				{
					// Góc ném xiên
					StarItem::getInstance()->alpha = 0;
					// Vận tốc ban đầu
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
	m_box = GameObject::getBouding();
	
	m_box.x += 2;
	m_box.width -= 4;

	return m_box;
}

bool ItemInBox::isCollision(GameObject* gameObject)
{
	// Mario
	Mario* mario = dynamic_cast<Mario*>(gameObject);
	if (mario != nullptr)
	{
		if (m_isActive &&
			m_status == StatusObject::ALIVE &&
			m_type != LuckyBoxsType::IT_COIN)
		{
			if (Collision::getInstance()->isCollision(mario, this) != DIR::NONE)
			{
				mario->setVelocity(Collision::getInstance()->getVelocity());
				m_status = StatusObject::DEAD;

				// update state for mario
				if (m_type == LuckyBoxsType::IT_MUSHROOM_BIGGER)
				{
					mario->setCanShoot(false);
					mario->setIsBig(true);
				}
				else if (m_type == LuckyBoxsType::IT_STAR)
					mario->setStar(true);
				else if (m_type == LuckyBoxsType::IT_GUN)
				{
					mario->setCanShoot(true);
					mario->setIsBig(false);
				}
				else if (m_type == LuckyBoxsType::IT_MUSHROOM_UP)
					mario->setLives(mario->getLives() + 1);
			}
		}
	}




	// item di chuyển trên game
	int type = gameObject->getTypeObject();
	if (type == TypeObject::Dynamic_TiledMap // Tiled map
		|| type == TypeObject::Moving_Enemy // Enemy  
		|| !m_isActive // item hiện đang không active
		|| m_stateMachine->isInState(*ItemInLuckyBoxIdle::getInstance()) // Item trong trạng thái move up
		|| m_type == LuckyBoxsType::IT_COIN || m_type == LuckyBoxsType::IT_GUN) // Super mushroom, 1-Up, Super star
		return false;

	ItemInBox* itemInBox = dynamic_cast<ItemInBox*>(gameObject);
	if (itemInBox != nullptr && itemInBox->getItemType() == LuckyBoxsType::IT_COIN)
		return false;


	DIR dir = Collision::getInstance()->isCollision(this, gameObject);
	if (dir != DIR::NONE)
	{
		m_velocity = Collision::getInstance()->getVelocity();
		
		if (dir == DIR::LEFT)
			m_flip = SpriteEffect::Flip;
		else if (dir == DIR::RIGHT)
			m_flip = SpriteEffect::None;
		
		// hướng để dùng cho super star
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

void ItemInBox::setCurrentFrame(int frame)
{
	m_currentFrame = frame;
	m_width = abs(m_frameList[m_currentFrame].rect.right - m_frameList[m_currentFrame].rect.left);
	m_height = abs(m_frameList[m_currentFrame].rect.bottom - m_frameList[m_currentFrame].rect.top);
}