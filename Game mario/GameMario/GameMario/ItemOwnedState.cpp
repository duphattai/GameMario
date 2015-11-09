#include "ItemOwnedState.h"
#include "ReSource.h"

// hiệu ứng di chuyển lên xuống luckybox
#pragma region LuckyBox

IdleLuckyBox* IdleLuckyBox::m_instance = 0;

IdleLuckyBox* IdleLuckyBox::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new IdleLuckyBox();
	}

	return m_instance;
}

void IdleLuckyBox::enter(LuckyBox* item)
{
	item->setVelocity(Vector2(0, 0));

	if (item->getCountItem() == 0)
	{
		item->setSpriteSheet(ReSource::getInstance()->getSprite(IDImage::IMG_TILEMAP));
		item->setCurrentFrame(item->getSizeFrameList() - 1);// hard code
	}

	item->setTimeAnimation(5);
}

void IdleLuckyBox::execute(LuckyBox* item)
{
	// animation
	int time = item->getTimeAnimation();
	if (item->getTimeAnimation() == 0 && item->getCountItem() > 0)
	{
		int index = item->getCurrentFrame() + 1;
		if (index == item->getSizeFrameList() - 1)
			index = 0;

		item->setCurrentFrame(index);
		item->setTimeAnimation(5);
	}
	item->setTimeAnimation(item->getTimeAnimation() - 1);

	// change state make effect
	if (item->getMakeEffect() && item->getCountItem() > 0)
	{
		item->getStateMachine()->changeState(LuckyBoxEffect::getInstance());
		item->setCountItem(item->getCountItem() - 1);
	}
}

void IdleLuckyBox::exit(LuckyBox* item)
{
	item->setMakeEffect(false);
}



///// effect when box is collided
LuckyBoxEffect* LuckyBoxEffect::m_instance = 0;

LuckyBoxEffect* LuckyBoxEffect::getInstance()
{
	if (m_instance == NULL)
		m_instance = new LuckyBoxEffect();

	return m_instance;
}

void LuckyBoxEffect::enter(LuckyBox* item)
{
	item->setTimeAnimation(5);
	item->setVelocity(Vector2(0, 3));
}

void LuckyBoxEffect::execute(LuckyBox* item)
{
	if (item->getCountItem() == 0)
	{
		item->setSpriteSheet(ReSource::getInstance()->getSprite(IDImage::IMG_TILEMAP));
		item->setCurrentFrame(item->getSizeFrameList() - 1);// hard code
	}
	else
	{
		// animation
		if (item->getTimeAnimation() == 0 && item->getCountItem() > 0)
		{
			int index = item->getCurrentFrame() + 1;
			if (index == item->getSizeFrameList() - 1)
				index = 0;

			item->setCurrentFrame(index);
			item->setTimeAnimation(5);
		}
		item->setTimeAnimation(item->getTimeAnimation() - 1);
	}

	// update position
	Vector2 velocity = item->getVelocity();
	velocity.y -= 1;
	item->setVelocity(velocity);

	if (velocity.y < -3)
		item->getStateMachine()->changeState(IdleLuckyBox::getInstance());
	else if (velocity.y == 0)
	{
		item->getItem()->setActive(true);
		item->getItem()->setPosition(item->getPosition().x, item->getPosition().y);

		// playsound
		//Tài: 9/11 play sound
		if (item->getType() == ItemsType::IT_COIN)
			PlaySound(L"Sounds/smb_coin.wav", NULL, SND_ASYNC);
		else if (item->getType() == ItemsType::IT_MUSHROOM_BIGGER)
			PlaySound(L"Sounds/smb_powerup_appears.wav", NULL, SND_ASYNC);
		else if (item->getType() == ItemsType::IT_MUSHROOM_UP)
			PlaySound(L"Sounds/smb_powerup_appears.wav", NULL, SND_ASYNC);
	}
}

void LuckyBoxEffect::exit(LuckyBox* item)
{
	item->setVelocity(Vector2(0, 0));
}


#pragma endregion


// hiệu ứng item đi lên và show ra số điểm
#pragma region Item in luckybox
//////////// item in luckybox move up

ItemInLuckyBoxIdle* ItemInLuckyBoxIdle::m_instance = 0;

ItemInLuckyBoxIdle* ItemInLuckyBoxIdle::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new ItemInLuckyBoxIdle();
	}

	return m_instance;
}

void ItemInLuckyBoxIdle::enter(ItemInBox* item)
{
	item->setTimeAnimation(5);
	item->setVelocity(Vector2(0, 5));
}

void ItemInLuckyBoxIdle::execute(ItemInBox* item)
{
	if (item->isActive())
	{
		Vector2 velocity = item->getVelocity();
		velocity.y--;
		item->setVelocity(velocity);

		if (velocity.y <= 0)
		{
			switch (item->getItemsType())
			{
				case ItemsType::IT_COIN:
					item->getStateMachine()->changeState(CoinInLuckyBox::getInstance());
					break;
				case ItemsType::IT_GUN:
					item->getStateMachine()->changeState(FlowerGun::getInstance());
					break;
				case ItemsType::IT_MUSHROOM_BIGGER:
					item->getStateMachine()->changeState(MoveMushroomItem::getInstance());
					break;
				case ItemsType::IT_MUSHROOM_UP:
					item->getStateMachine()->changeState(MoveMushroomItem::getInstance());
					break;
				case ItemsType::IT_STAR:
					item->getStateMachine()->changeState(StarItem::getInstance());
					break;
			}
		}
	}
}

void ItemInLuckyBoxIdle::exit(ItemInBox* item)
{
}

///////////////////////// item in luckybox score

ItemInLuckyBoxScore* ItemInLuckyBoxScore::m_instance = 0;

ItemInLuckyBoxScore* ItemInLuckyBoxScore::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new ItemInLuckyBoxScore();
	}

	return m_instance;
}

void ItemInLuckyBoxScore::enter(ItemInBox* item)
{
	item->setVelocity(Vector2(0, 5));

	switch (item->getItemsType())
	{
	case ItemsType::IT_COIN:
		item->setText(L"200");
		break;
	case ItemsType::IT_GUN:
		item->setText(L"1000");
		break;
	case ItemsType::IT_MUSHROOM_BIGGER:
		item->setText(L"1000");
		break;
	case ItemsType::IT_MUSHROOM_UP:
		item->setText(L"1 UP");
		break;
	case ItemsType::IT_STAR:
		item->setText(L"1000");
		break;
	}
}

void ItemInLuckyBoxScore::execute(ItemInBox* item)
{
	if (item->isActive())
	{
		Vector2 velocity = item->getVelocity();
		if (velocity.y < 0)
		{
			item->setActive(false);
			item->getStateMachine()->changeState(ItemInLuckyBoxIdle::getInstance());

			return;
		}
		
		velocity.y--;
		item->setVelocity(velocity);
	}
}

void ItemInLuckyBoxScore::exit(ItemInBox* item)
{
	item->setText(L"");
	item->setStatusObject(StatusObject::ALIVE);
}

#pragma endregion


/////////////////
FlowerGun* FlowerGun::m_instance = 0;

FlowerGun* FlowerGun::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new FlowerGun();
	}

	return m_instance;
}

void FlowerGun::enter(ItemInBox* item)
{
	item->setCurrentFrame(0);
	item->setTimeAnimation(5);
	item->setVelocity(Vector2(0, 0));
}

void FlowerGun::execute(ItemInBox* item)
{
	if (item->isActive())
	{
		if (item->getStatusOBject() == StatusObject::ALIVE)
		{
			// update velocity
			item->setTimeAnimation(item->getTimeAnimation() - 1);
			if (item->getTimeAnimation() <= 0)
			{
				// update animation gun
				int currentIndex = item->getCurrentFrame() + 1;
				if (currentIndex == item->getSizeFrameList())
					currentIndex = 0;
				item->setCurrentFrame(currentIndex);

				item->setTimeAnimation(5);
			}
		}
		else
		{
			item->getStateMachine()->changeState(ItemInLuckyBoxScore::getInstance());
			return;
		}
	}
}

void FlowerGun::exit(ItemInBox* item)
{
}


/////////////////////////////// mush room
MoveMushroomItem* MoveMushroomItem::m_instance = 0;

MoveMushroomItem* MoveMushroomItem::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new MoveMushroomItem();
	}

	return m_instance;
}

void MoveMushroomItem::enter(ItemInBox* item)
{
	item->setCurrentFrame(0);
	item->setVelocity(Vector2(0, GRAVITATION));
}

void MoveMushroomItem::execute(ItemInBox* item)
{
	if (item->isActive())
	{
		// update velocity
		Vector2 velocity = item->getVelocity();
		if (item->getStatusOBject() == StatusObject::DEAD)
		{
			item->getStateMachine()->changeState(ItemInLuckyBoxScore::getInstance());
			return;
		}
		else
		{
			velocity.y += GRAVITATION;
			if (item->getFliping() == SpriteEffect::None)
				velocity.x = 2;
			else
				velocity.x = -2;

			item->setVelocity(velocity);
		}
	}
}

void MoveMushroomItem::exit(ItemInBox* item)
{
}



///////////////////////// Coin in box
CoinInLuckyBox* CoinInLuckyBox::m_instance = 0;

CoinInLuckyBox* CoinInLuckyBox::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new CoinInLuckyBox();
	}

	return m_instance;
}

void CoinInLuckyBox::enter(ItemInBox* item)
{
	item->setCurrentFrame(0);
	item->setTimeAnimation(3);
	item->setVelocity(Vector2(0, 7));
}

void CoinInLuckyBox::execute(ItemInBox* item)
{
	if (item->isActive())
	{
		Vector2 velocity = item->getVelocity();
		if (item->getStatusOBject() == StatusObject::DEAD)
		{
			item->getStateMachine()->changeState(ItemInLuckyBoxScore::getInstance());
			return;
		}
		else
		{
			// update velocity, tạo hiệu ứng của coin
			if (velocity.y <= -5)
			{
				item->setStatusObject(StatusObject::DEAD);
				velocity.y = 0;
			}
			else
			{	
				velocity.y -= 1;
				// animation coin
				item->setTimeAnimation(item->getTimeAnimation() - 1);
				if (item->getTimeAnimation() <= 0)
				{
					int currentIndex = item->getCurrentFrame() + 1;
					if (currentIndex == item->getSizeFrameList())
						currentIndex = 0;
					item->setCurrentFrame(currentIndex);

					item->setTimeAnimation(3);
				}
			}

			item->setVelocity(velocity);
		}
	}
}

void CoinInLuckyBox::exit(ItemInBox* item)
{
}


//////////////////////////////// star
StarItem* StarItem::m_instance = 0;

StarItem* StarItem::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new StarItem();
	}

	return m_instance;
}

void StarItem::enter(ItemInBox* item)
{
	alpha = alpha = 3.14 / 3;
	v = 6;

	item->setCurrentFrame(0);
	item->setTimeAnimation(5);
	item->setVelocity(Vector2(0, 0));

	item->m_time = 0;
}

void StarItem::execute(ItemInBox* item)
{
	if (item->isActive())
	{
		Vector2 velocity = item->getVelocity();
		if (item->getStatusOBject() == StatusObject::DEAD)
		{
			item->getStateMachine()->changeState(ItemInLuckyBoxScore::getInstance());
			return;
		}
		else
		{
			// update velocity, tạo hiệu ứng của coin
			velocity = item->m_mathematical(item->m_time++, v, alpha);
			if (item->getFliping() == SpriteEffect::Flip)
				velocity.x *= -1;

			// animation coin
			item->setTimeAnimation(item->getTimeAnimation() - 1);
			if (item->getTimeAnimation() <= 0)
			{
				int currentIndex = item->getCurrentFrame() + 1;
				if (currentIndex == item->getSizeFrameList())
					currentIndex = 0;
				item->setCurrentFrame(currentIndex);

				item->setTimeAnimation(5);
			}
			
			item->setVelocity(velocity);
		}
	}
}

void StarItem::exit(ItemInBox* item)
{
}


#pragma region Animation of brick


//////////////////// idle brick
BrickIdle* BrickIdle::m_instance = 0;

BrickIdle* BrickIdle::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new BrickIdle();
	}

	return m_instance;
}

void BrickIdle::enter(Brick* brick)
{
	brick->setSpriteSheet(ReSource::getInstance()->getSprite(IDImage::IMG_TILEMAP));
	brick->setIndexSprite(1); // hard code
	brick->setVelocity(Vector2(0, 0));
}

void BrickIdle::execute(Brick* brick)
{
	if (brick->isActive())
	{
		if (brick->getMakeEffect())
		{
			brick->getStateMachine()->changeState(BrickEffect::getInstance());
		}
		else if (brick->isBreak())
		{
			brick->getStateMachine()->changeState(BrickBroken::getInstance());
		}
	}
}

void BrickIdle::exit(Brick* brick)
{
}

///////////////////// brick effect
BrickEffect* BrickEffect::m_instance = 0;

BrickEffect* BrickEffect::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new BrickEffect();
	}

	return m_instance;
}

void BrickEffect::enter(Brick* brick)
{
	brick->setSpriteSheet(ReSource::getInstance()->getSprite(IDImage::IMG_TILEMAP));
	brick->setIndexSprite(1); // hard code
	brick->setVelocity(Vector2(0, 3));
	brick->setMakeEffect(false);

	PlaySound(L"Sounds/smb_kick.wav", NULL, SND_ASYNC);
}

void BrickEffect::execute(Brick* brick)
{
	if (brick->isActive())
	{
		Vector2 velocity = brick->getVelocity();
		velocity.y--;
		brick->setVelocity(velocity);
		if (velocity.y < -3)
		{
			brick->getStateMachine()->changeState(BrickIdle::getInstance());
		}
	}
}

void BrickEffect::exit(Brick* brick)
{
}

/////////////////////// brick broken
BrickBroken* BrickBroken::m_instance = 0;

BrickBroken* BrickBroken::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new BrickBroken();
	}

	return m_instance;
}

void BrickBroken::enter(Brick* brick)
{
	brick->setSpriteSheet(ReSource::getInstance()->getSprite(IDImage::IMG_ITEMSHEET));
	brick->setCurrentFrame(0);
	brick->setVelocity(Vector2(0, 0));

	// play sound
	PlaySound(L"Sounds/smb_breakblock.wav", NULL, SND_ASYNC);
}

void BrickBroken::execute(Brick* brick)
{
	if (brick->isActive())
	{
		Vector2 velocity;
		int time = brick->m_time;
		vector<Vector2> framePosition = brick->getFramePosition();

		velocity = vanTocNemXien(time, 6, 3.14 / 3); // 60 độ, sang phải
		framePosition[0].x += velocity.x;
		framePosition[0].y += velocity.y;

		velocity = vanTocNemXien(time, 8, 7 * 3.14 / 18); // 70 độ, sang phải
		framePosition[1].x += velocity.x;
		framePosition[1].y += velocity.y;

		velocity = vanTocNemXien(time, 6, 2 * 3.14 / 3); // 120 độ, sang trái
		framePosition[2].x += velocity.x;
		framePosition[2].y += velocity.y;

		velocity = vanTocNemXien(time, 8, 11 * 3.14 / 18); // 110 độ, sang trái
		framePosition[3].x += velocity.x;
		framePosition[3].y += velocity.y;
		brick->setFramePosition(framePosition);

		// cập nhật aniamtion
		int timeAnimation = brick->getTimeAnimation() - 1;
		if (timeAnimation == 0)
		{
			int m_currentFrame = brick->getCurrentFrame() + 1;
			if (m_currentFrame > brick->getSizeOfFrameList() - 1)
				m_currentFrame = 0;
			timeAnimation = 4;
		}
		brick->setTimeAnimation(timeAnimation);
		brick->m_time++;
		if (brick->m_time >= 20)
			brick->setActive(false);
	}
}

void BrickBroken::exit(Brick* brick)
{
	brick->setSpriteSheet(ReSource::getInstance()->getSprite(IDImage::IMG_TILEMAP));
}

#pragma endregion