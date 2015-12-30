#include "BulletFireOwnedState.h"
#include "SoundClass.h"

BulletFireIdle* BulletFireIdle::m_instance = 0;

BulletFireIdle* BulletFireIdle::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new BulletFireIdle();
	}

	return m_instance;
}

void BulletFireIdle::enter(BulletFire* bullet)
{
	bullet->setActive(false);
	bullet->setExplode(false);
	bullet->setCurrentFrame(0);
	bullet->setVelocity(Vector2(0, 0));
}

void BulletFireIdle::execute(BulletFire* bullet)
{
	if (bullet->isActive())
		bullet->getStateMachine()->changeState(BulletFireMoving::getInstance());
}

void BulletFireIdle::exit(BulletFire* bullet)
{
}

void BulletFireIdle::release()
{
	delete m_instance;
	m_instance = NULL;
}

/////////////////////
BulletFireMoving* BulletFireMoving::m_instance = 0;

BulletFireMoving* BulletFireMoving::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new BulletFireMoving();
	}

	return m_instance;
}

void BulletFireMoving::enter(BulletFire* bullet)
{
	bullet->setCurrentFrame(0);
	bullet->setTimeToLive(50);
	bullet->setTimeAnimation(2);
}

void BulletFireMoving::execute(BulletFire* bullet)
{
	if (bullet->isExplode())
	{
		bullet->getStateMachine()->changeState(BulletFireExplode::getInstance());
	}
	else
	{
		// update velocity
		Vector2 velocity = Vector2(3,0);
		if (bullet->getFliping() == SpriteEffect::Flip)
			velocity.x *= -1;
		bullet->setVelocity(velocity);

		// cập nhật animation
		int timeAnimation = bullet->getTimeAnimation() - 1;
		if (timeAnimation < 0)
		{
			int currentFrame = bullet->getCurrentFrame() + 1;
			if (currentFrame >= 2) // hard code
				currentFrame = 0;

			bullet->setCurrentFrame(currentFrame);

			timeAnimation = 2;
		}
		bullet->setTimeAnimation(timeAnimation);


		// chuyển trạng thái
		bullet->setTimeToLive(bullet->getTimeToLive() - 1);
		if (bullet->getTimeToLive() <= 0)
			bullet->getStateMachine()->changeState(BulletFireIdle::getInstance());
		else if (bullet->isExplode())
			bullet->getStateMachine()->changeState(BulletFireExplode::getInstance());
	}
}

void BulletFireMoving::exit(BulletFire* bullet)
{
}

void BulletFireMoving::release()
{
	delete m_instance;
	m_instance = NULL;
}

///////////////////////
BulletFireExplode* BulletFireExplode::m_instance = 0;

BulletFireExplode* BulletFireExplode::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new BulletFireExplode();
	}

	return m_instance;
}


void BulletFireExplode::enter(BulletFire* bullet)
{
	bullet->setCurrentFrame(2);
	bullet->setVelocity(Vector2());
	SoundClass::getInstance()->playWaveFile(IDSounds::Sound_FireWorks);
}

void BulletFireExplode::execute(BulletFire* bullet)
{
	int currentFrame = bullet->getCurrentFrame() + 1;
	if (currentFrame >= bullet->getSizeOfFrameList())
	{
		bullet->getStateMachine()->changeState(BulletFireIdle::getInstance());
	}
	else
		bullet->setCurrentFrame(currentFrame);
}

void BulletFireExplode::exit(BulletFire* bullet)
{
	bullet->setActive(false);
	bullet->setCurrentFrame(0);
}

void BulletFireExplode::release()
{
	delete m_instance;
	m_instance = NULL;
}