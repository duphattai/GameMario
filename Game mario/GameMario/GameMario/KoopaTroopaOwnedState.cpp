#include "KoopaTroopaOwnedState.h"
#include "ScoreGame.h"
#include "SoundClass.h"

KoopaTroopaMove* KoopaTroopaMove::m_instance = 0;

KoopaTroopaMove::KoopaTroopaMove()
{
	for (int i = 0; i < 2; i++)
		m_frameAnimation.push_back(Frame(96 + 16 * i, 8, 16, 24));
}

KoopaTroopaMove* KoopaTroopaMove::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new KoopaTroopaMove();
	}

	return m_instance;
}

void KoopaTroopaMove::enter(Enemy* enemy)
{
	enemy->setAttack(BeAttack::None);
	enemy->setFrameList(m_frameAnimation);
	enemy->setVelocity(Vector2(0, 0));
	enemy->setTimeAnimation(5);
	enemy->setCurrentFrame(0);
}

void KoopaTroopaMove::execute(Enemy* enemy)
{
	// update velocity rùa di chuyển index = 0 và 1
	Vector2 velocity = enemy->getVelocity();
	velocity = Vector2(2, 2 * GRAVITATION);
	//Update velocity hướng va chạm
	if (enemy->getFliping() == SpriteEffect::None)
		velocity.x *= -1;
	enemy->setVelocity(velocity);


	//update animation
	int current = enemy->getCurrentFrame() + 1;
	if (current >= enemy->getSizeFrameList())//cập nhật indext
		current = 0;
	int time = enemy->getTimeAnimation() - 1;
	if (time == 0)
	{
		enemy->setCurrentFrame(current);
		time = 5;
	}
	enemy->setTimeAnimation(time);


	// update state
	if (enemy->getAttack() == BeAttack::DeathByJump)
	{
		enemy->getStateMachine()->changeState(KoopaTroopaShellNotMove::getInstance());
	}
	else if (enemy->getAttack() == BeAttack::DeathByGun)
	{
		enemy->getStateMachine()->changeState(KoopaTroopaDieByGun::getInstance());
	}
}

void KoopaTroopaMove::exit(Enemy* enemy)
{
}

//State mai rùa khi mario nhảy lên
KoopaTroopaShellNotMove* KoopaTroopaShellNotMove::m_instance = 0;

KoopaTroopaShellNotMove::KoopaTroopaShellNotMove()
{
	m_frameAnimation.push_back(Frame(161, 17, 14, 12));
}

KoopaTroopaShellNotMove* KoopaTroopaShellNotMove::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new KoopaTroopaShellNotMove();
	}

	return m_instance;
}

void KoopaTroopaShellNotMove::enter(Enemy* enemy)
{
	enemy->setFrameList(m_frameAnimation);
	enemy->setCurrentFrame(0);
	enemy->setVelocity(Vector2(0, 0));
	enemy->setAttack(BeAttack::None);
	enemy->setTimeAnimation(300);
	SoundClass::getInstance()->playWaveFile(IDSounds::Sound_Stomp);
}

void KoopaTroopaShellNotMove::execute(Enemy* enemy)
{
	// update velocity rùa di chuyển index = 0 và 1
	Vector2 velocity = enemy->getVelocity();
	velocity = Vector2(0, 0);
	//Update velocity hướng va chạm
	enemy->setVelocity(velocity);

	//update animation
	int current = enemy->getCurrentFrame() + 1;
	if (current >= enemy->getSizeFrameList())//cập nhật indext
		current = 0;
	enemy->setCurrentFrame(current);// index của hình mai rùa 

	// update state
	int time = enemy->getTimeAnimation() - 1;
	enemy->setTimeAnimation(time);
	if (time <= 20 && time > 0)
		enemy->setCurrentFrame(3);
	else if (time <= 0)
	{
		enemy->getStateMachine()->changeState(KoopaTroopaMove::getInstance());
	}
	else if (enemy->getAttack() == BeAttack::DeathByJump)
	{
		enemy->getStateMachine()->changeState(KoopaTroopaShellMove::getInstance());
	}
	else if (enemy->getAttack() == BeAttack::DeathByGun)
	{
		enemy->setCurrentFrame(2);
		enemy->getStateMachine()->changeState(KoopaTroopaDieByGun::getInstance());
	}
}

void KoopaTroopaShellNotMove::exit(Enemy* enemy)
{
}

//Enemy Die by gun
KoopaTroopaDieByGun* KoopaTroopaDieByGun::m_instance = 0;

KoopaTroopaDieByGun* KoopaTroopaDieByGun::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new KoopaTroopaDieByGun();
	}

	return m_instance;
}

void KoopaTroopaDieByGun::enter(Enemy* enemy)
{
	enemy->setVelocity(Vector2(0, 6));
	enemy->setStatusObject(StatusObject::DEAD);
	enemy->setTimeAnimation(120);
	enemy->setText(L"200");
	enemy->setPositionText(enemy->getPosition());
	ScoreGame::getInstance()->setScore(ScoreGame::getInstance()->getScore() + 100);
	SoundClass::getInstance()->playWaveFile(IDSounds::Sound_Stomp);
}

void KoopaTroopaDieByGun::execute(Enemy* enemy)
{
	Vector2 velocity = enemy->getVelocity();
	velocity.y--;
	enemy->setVelocity(velocity);
	enemy->setTimeAnimation(enemy->getTimeAnimation() - 1);
	if (enemy->getTimeAnimation() <= 0)
		enemy->setActive(false);
}

void KoopaTroopaDieByGun::exit(Enemy* enemy)
{
}


// Shell move

KoopaTroopaShellMove* KoopaTroopaShellMove::m_instance = 0;

KoopaTroopaShellMove::KoopaTroopaShellMove()
{
	m_frameAnimation.push_back(Frame(161, 17, 14, 12));
}

KoopaTroopaShellMove* KoopaTroopaShellMove::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new KoopaTroopaShellMove();
	}

	return m_instance;
}

void KoopaTroopaShellMove::enter(Enemy* enemy)
{
	enemy->setVelocity(Vector2(0, 6));
	enemy->setFrameList(m_frameAnimation);
	enemy->setCurrentFrame(0);
}

void KoopaTroopaShellMove::execute(Enemy* enemy)
{
	// update velocity rùa di chuyển index = 0 và 1
	Vector2 velocity = enemy->getVelocity();
	velocity = Vector2(6, 2 * GRAVITATION);
	//Update velocity hướng va chạm
	if (enemy->getFliping() == SpriteEffect::None)
		velocity.x *= -1;
	enemy->setVelocity(velocity);

	//update animation
	int current = enemy->getCurrentFrame() + 1;
	if (current >= enemy->getSizeFrameList())//cập nhật indext
		current = 0;
	enemy->setCurrentFrame(current);// index của hình mai rùa 

	if (enemy->getAttack() == BeAttack::DeathByGun)
	{
		enemy->setPositionText(enemy->getPosition());
		enemy->getStateMachine()->changeState(KoopaTroopaDieByGun::getInstance());
	}
}

void KoopaTroopaShellMove::exit(Enemy* enemy)
{
}
