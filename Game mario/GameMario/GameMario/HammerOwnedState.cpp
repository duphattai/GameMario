#include "HammerOwnedState.h"
#include "SoundClass.h"

HammerIdle* HammerIdle::m_instance = 0;

HammerIdle* HammerIdle::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new HammerIdle();
	}

	return m_instance;
}

void HammerIdle::enter(Hammer* bullet)
{
	bullet->setActive(false);
	bullet->setExplode(false);
	bullet->setCurrentFrame(0);
	bullet->setVelocity(Vector2(0, 0));
}

void HammerIdle::execute(Hammer* bullet)
{
	if (bullet->isActive())
		bullet->getStateMachine()->changeState(HammerMoving::getInstance());
}

void HammerIdle::exit(Hammer* bullet)
{
}
void HammerIdle::release()
{
	delete m_instance;
	m_instance = NULL;
}


/////////////////////

HammerMoving* HammerMoving::m_instance = 0;

HammerMoving* HammerMoving::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new HammerMoving();
	}

	return m_instance;
}

void HammerMoving::enter(Hammer* bullet)
{
	bullet->setCurrentFrame(0);
	bullet->time = 0;
	bullet->setTimeToLive(50);
	bullet->setTimeAnimation(2);
	bullet->alpha = 3.14 / 3;
}

void HammerMoving::execute(Hammer* bullet)
{
	if (bullet->isExplode())
	{
		bullet->getStateMachine()->changeState(HammerExplode::getInstance());
	}
	else
	{
		// update velocity
		Vector2 velocity = vanTocNemXien(bullet->time, 8, bullet->alpha);
		bullet->time++;
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
			bullet->getStateMachine()->changeState(HammerIdle::getInstance());
		else if (bullet->isExplode())
			bullet->getStateMachine()->changeState(HammerExplode::getInstance());
	}
}

void HammerMoving::exit(Hammer* bullet)
{
}

void HammerMoving::release()
{
	delete m_instance;
	m_instance = NULL;
}
///////////////////
HammerExplode* HammerExplode::m_instance = 0;

HammerExplode* HammerExplode::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new HammerExplode();
	}

	return m_instance;
}


void HammerExplode::enter(Hammer* bullet)
{
	bullet->setCurrentFrame(4);
	bullet->setVelocity(Vector2());
	SoundClass::getInstance()->playWaveFile(IDSounds::Sound_FireWorks);
}

void HammerExplode::execute(Hammer* bullet)
{
	int currentFrame = bullet->getCurrentFrame() + 1;
	if (currentFrame >= bullet->getSizeOfFrameList())
	{
		bullet->getStateMachine()->changeState(HammerIdle::getInstance());
	}
	else
		bullet->setCurrentFrame(currentFrame);
}

void HammerExplode::exit(Hammer* bullet)
{
	bullet->setActive(false);
	bullet->setCurrentFrame(0);
}
void HammerExplode::release()
{
	delete m_instance;
	m_instance = NULL;
}