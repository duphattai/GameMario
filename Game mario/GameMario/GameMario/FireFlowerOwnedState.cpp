#include "FireFlowerOwnedState.h"
#include "ScoreGame.h"
#include "SoundClass.h"
#include "FireFlower.h"

//////////////////////////////////////////
FireFlowerMove* FireFlowerMove::m_instance = 0;

FireFlowerMove* FireFlowerMove::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new FireFlowerMove();
	}

	return m_instance;
}

void FireFlowerMove::enter(Enemy* enemy)
{
	enemy->setVelocity(Vector2(0, GRAVITATION));
	enemy->setTimeAnimation(5);
	enemy->setCurrentFrame(0);
}

void FireFlowerMove::execute(Enemy* enemy)
{
	// update velocity nam di chuyển index = 0 và 1
	Vector2 velocity = enemy->getVelocity();
	FireFlower* fireFlower = dynamic_cast<FireFlower*>(enemy);
	fireFlower->m_timeMove--;
	if (fireFlower->m_timeMove == 30)
	{
		velocity.y /= 30;
	}
	else if (fireFlower->m_timeMove <= 0)
	{
		velocity.y *= 30;
		velocity.y *= -1;
		fireFlower->m_timeMove = 80;
	}
	enemy->setVelocity(velocity);

	//update animation
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

	if (enemy->getAttack() == BeAttack::DeathByGun)
		enemy->getStateMachine()->changeState(FireFlowerDieByGun::getInstance());
}

void FireFlowerMove::exit(Enemy* enemy)
{
}

////////////////////////////////////
FireFlowerDieByGun* FireFlowerDieByGun::m_instance = 0;

FireFlowerDieByGun* FireFlowerDieByGun::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new FireFlowerDieByGun();
	}

	return m_instance;
}

void FireFlowerDieByGun::enter(Enemy* enemy)
{
	enemy->setAlphaColor(D3DCOLOR_ARGB(0, 255, 255, 255));
	enemy->setStatusObject(StatusObject::DEAD);
	enemy->setTimeAnimation(50);
	enemy->setText(L"200");
	enemy->setPositionText(enemy->getPosition());
	ScoreGame::getInstance()->setScore(ScoreGame::getInstance()->getScore() + 100);
	SoundClass::getInstance()->playWaveFile(IDSounds::Sound_Stomp);
}

void FireFlowerDieByGun::execute(Enemy* enemy)
{
	enemy->setTimeAnimation(enemy->getTimeAnimation() - 1);
	if (enemy->getTimeAnimation() <= 0)
		enemy->setActive(false);
}

void FireFlowerDieByGun::exit(Enemy* enemy)
{
}
