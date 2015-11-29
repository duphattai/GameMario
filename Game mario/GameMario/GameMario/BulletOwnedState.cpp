#include "BulletOwnedState.h"
#include "SoundClass.h"

BulletIdle* BulletIdle::m_instance = 0;

BulletIdle* BulletIdle::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new BulletIdle();
	}

	return m_instance;
}

void BulletIdle::enter(Bullet* bullet)
{
	bullet->setActive(false);
	bullet->setExplode(false);
	bullet->setCurrentFrame(0);
	bullet->setVelocity(Vector2(0, 0));
}

void BulletIdle::execute(Bullet* bullet)
{
	if (bullet->isActive())
		bullet->getStateMachine()->changeState(BulletMoving::getInstance());
}

void BulletIdle::exit(Bullet* bullet)
{
}


//////////////
BulletMoving* BulletMoving::m_instance = 0;

BulletMoving* BulletMoving::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new BulletMoving();
	}

	return m_instance;
}

void BulletMoving::enter(Bullet* bullet)
{
	bullet->setCurrentFrame(0);
	bullet->time = 0;
	bullet->alpha = -3.14 / 4;
	bullet->setTimeToLive(50);
	bullet->setTimeAnimation(2);

	// play sound
	SoundClass::getInstance()->playWaveFile(IDSounds::Sound_FireBall);
}

void BulletMoving::execute(Bullet* bullet)
{
	if (bullet->isExplode())
	{
		bullet->getStateMachine()->changeState(BulletExplode::getInstance());
	}
	else
	{
		// update velocity
		Vector2 velocity = vanTocNemXien(bullet->time, 9, bullet->alpha);
		bullet->time += 1.5;
		if (bullet->getFliping() == SpriteEffect::Flip)
			velocity.x *= -1;
		bullet->setVelocity(velocity);

		// cập nhật animation
		int timeAnimation = bullet->getTimeAnimation() - 1;
		if (timeAnimation < 0)
		{
			int currentFrame = bullet->getCurrentFrame() + 1;
			if (currentFrame >= 4) // hard code
				currentFrame = 0;

			bullet->setCurrentFrame(currentFrame);

			timeAnimation = 2;
		}
		bullet->setTimeAnimation(timeAnimation);
		

		// chuyển trạng thái
		bullet->setTimeToLive(bullet->getTimeToLive() - 1);
		if (bullet->getTimeToLive() <= 0)
		{
			bullet->getStateMachine()->changeState(BulletIdle::getInstance());
		}
		else if (bullet->isExplode())
		{
			bullet->getStateMachine()->changeState(BulletExplode::getInstance());
		}
	}
}

void BulletMoving::exit(Bullet* bullet)
{
}


///////////////////
BulletExplode* BulletExplode::m_instance = 0;

BulletExplode* BulletExplode::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new BulletExplode();
	}

	return m_instance;
}


void BulletExplode::enter(Bullet* bullet)
{
	bullet->setCurrentFrame(4);
	bullet->setVelocity(Vector2());
	SoundClass::getInstance()->playWaveFile(IDSounds::Sound_FireWorks);
}

void BulletExplode::execute(Bullet* bullet)
{
	int currentFrame = bullet->getCurrentFrame() + 1;
	if (currentFrame == bullet->getSizeOfFrameList())
	{
		bullet->getStateMachine()->changeState(BulletIdle::getInstance());
	}
	else
		bullet->setCurrentFrame(currentFrame);
}

void BulletExplode::exit(Bullet* bullet)
{
	bullet->setActive(false);
	bullet->setCurrentFrame(0);
}