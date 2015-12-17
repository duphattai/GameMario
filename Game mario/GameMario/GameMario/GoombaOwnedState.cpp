#include "GoombaOwnedState.h"
#include "ScoreGame.h"
#include "SoundClass.h"
//Enemy Die by gun
GoombaDieByGun* GoombaDieByGun::m_instance = 0;

GoombaDieByGun* GoombaDieByGun::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new GoombaDieByGun();
	}

	return m_instance;
}

void GoombaDieByGun::enter(Enemy* enemy)
{
	enemy->setVelocity(Vector2(0, 6));
	enemy->setStatusObject(StatusObject::DEAD);
	enemy->setTimeAnimation(120);
	enemy->setText(L"100");
	enemy->setPositionText(enemy->getPosition());
	ScoreGame::getInstance()->setScore(ScoreGame::getInstance()->getScore() + 100);
	SoundClass::getInstance()->playWaveFile(IDSounds::Sound_Stomp);
}

void GoombaDieByGun::execute(Enemy* enemy)
{
	Vector2 velocity = enemy->getVelocity();
	velocity.y--;
	enemy->setVelocity(velocity);
	enemy->setTimeAnimation(enemy->getTimeAnimation() - 1);
	if (enemy->getTimeAnimation() <= 0)
		enemy->setActive(false);
}

void GoombaDieByGun::exit(Enemy* enemy)
{
}


//nam di chuyen
GoombaMove* GoombaMove::m_instance = 0;

GoombaMove* GoombaMove::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new GoombaMove();
	}

	return m_instance;
}

void GoombaMove::enter(Enemy* enemy)
{
	enemy->setVelocity(Vector2(0, 0));
	enemy->setTimeAnimation(5);
	enemy->setCurrentFrame(0);
}

void GoombaMove::execute(Enemy* enemy)
{
	// Cập nhật van tốc
	Vector2 velocity = enemy->getVelocity();
	velocity = Vector2(1, 5 * GRAVITATION);
	// Xác định hướng di chuyển
	if (enemy->getFliping() == SpriteEffect::None)
		velocity.x *= -1;
	enemy->setVelocity(velocity);


	// Animation
	int current = enemy->getCurrentFrame() + 1;
	if (current > 1)//cập nhật indext
		current = 0;
	int time = enemy->getTimeAnimation() - 1;
	if (time <= 0)
	{
		enemy->setCurrentFrame(current);
		time = 5;
	}
	enemy->setTimeAnimation(time);

	// neu chuyen state
	if (enemy->getAttack() == BeAttack::DeathByGun)
		enemy->getStateMachine()->changeState(GoombaDieByGun::getInstance());
	else if (enemy->getAttack() == BeAttack::DeathByJump)
		enemy->getStateMachine()->changeState(GoombaDieByJump::getInstance());
}

void GoombaMove::exit(Enemy* enemy)
{
}

//nam khi bi Mario nhay len
GoombaDieByJump* GoombaDieByJump::m_instance = 0;

GoombaDieByJump* GoombaDieByJump::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new GoombaDieByJump();
	}

	return m_instance;
}

void GoombaDieByJump::enter(Enemy* enemy)
{
	enemy->setVelocity(Vector2(0, 0));
	enemy->setAttack(BeAttack::None);
	enemy->setTimeAnimation(10);
	enemy->setText(L"100");
	ScoreGame::getInstance()->setScore(ScoreGame::getInstance()->getScore() + 100);
	enemy->setPositionText(enemy->getPosition());
	SoundClass::getInstance()->playWaveFile(IDSounds::Sound_Stomp);
}

void GoombaDieByJump::execute(Enemy* enemy)
{
	//update animation
	enemy->setCurrentFrame(2);// index của mushroom die
	enemy->setTimeAnimation(enemy->getTimeAnimation() - 1);
	if (enemy->getTimeAnimation() <= 0)
		enemy->setActive(false);
}

void GoombaDieByJump::exit(Enemy* enemy)
{
}
