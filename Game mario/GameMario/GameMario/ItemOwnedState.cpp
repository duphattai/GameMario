#include "ItemOwnedState.h"
#include "ReSource.h"


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
	item->setVelocity(Vector2(0, 4));
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
	velocity.y--;
	item->setVelocity(velocity);

	if (velocity.y < -4)
	{
		item->getStateMachine()->changeState(IdleLuckyBox::getInstance());
		item->getItem()->setActive(true);
	}
}

void LuckyBoxEffect::exit(LuckyBox* item)
{
	item->setVelocity(Vector2(0, 0));
}
















/////////////
Gun* Gun::m_instance = 0;

Gun* Gun::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new Gun();
	}

	return m_instance;
}

void Gun::enter(ItemInBox* item)
{
	item->setCurrentFrame(0);
	item->setTimeAnimation(5);
}

void Gun::execute(ItemInBox* item)
{
	if (item->isActive())
	{
		// update velocity
		Vector2 velocity = item->getVelocity();
		if (velocity.y > 0)
			item->setVelocity(Vector2(velocity.x, velocity.y - 1));
		
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


		if (item->getCollision())
		{
			item->setActive(false);
			item->setCollision(false);
		}
	}
}


void Gun::exit(ItemInBox* item)
{
}