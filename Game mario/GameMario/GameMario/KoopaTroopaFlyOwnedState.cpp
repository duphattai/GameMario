#include "KoopaTroopaFlyOwnedState.h"
#include "KoopaTroopaFly.h"
#include "KoopaTroopaOwnedState.h"

KoopaTroopaFlyMoveFly* KoopaTroopaFlyMoveFly::m_instance = 0;

KoopaTroopaFlyMoveFly::KoopaTroopaFlyMoveFly()
{
	// <Xác định frame animation của koopatroopafly>
	for (int i = 0; i < 2; i++)
		m_frameAnimation.push_back(Frame(128 + 16 * i, 8, 16, 24));
}

KoopaTroopaFlyMoveFly* KoopaTroopaFlyMoveFly::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new KoopaTroopaFlyMoveFly();
	}

	return m_instance;
}

void KoopaTroopaFlyMoveFly::enter(Enemy* enemy)
{
	enemy->setVelocity(Vector2(0, 0));
	enemy->setTimeAnimation(5);
	enemy->setCurrentFrame(0);
	enemy->setFrameList(m_frameAnimation);

	dynamic_cast<KoopaTroopaFly*>(enemy)->time = 0;
	dynamic_cast<KoopaTroopaFly*>(enemy)->alpha = 3.14 / 3;
}

void KoopaTroopaFlyMoveFly::execute(Enemy* enemy)
{
	// update velocity rùa di chuyển index = 0 và 1
	Vector2 velocity = vanTocNemXien(dynamic_cast<KoopaTroopaFly*>(enemy)->time, 6, dynamic_cast<KoopaTroopaFly*>(enemy)->alpha);
	dynamic_cast<KoopaTroopaFly*>(enemy)->time += 1;
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
		enemy->getStateMachine()->changeState(KoopaTroopaMove::getInstance());
	}
	else if (enemy->getAttack() == BeAttack::DeathByGun)
	{
		enemy->getStateMachine()->changeState(KoopaTroopaDieByGun::getInstance());
	}
}

void KoopaTroopaFlyMoveFly::exit(Enemy* enemy)
{
}

