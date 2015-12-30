#include "BowserOwnedState.h"
#include "ScoreGame.h"
#include "SoundClass.h"
#include "Mario.h"
#include "Hammer.h"
#include "Bowser.h"
#include "Camera.h"
// Bowser die by gun
BowserDieByGun* BowserDieByGun::m_instance = 0;

BowserDieByGun* BowserDieByGun::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new BowserDieByGun();
	}

	return m_instance;
}

void BowserDieByGun::enter(Enemy* enemy)
{
	enemy->setVelocity(Vector2(0, 10));
	enemy->setStatusObject(StatusObject::DEAD);
	enemy->setTimeAnimation(120);
	enemy->setText(L"1000");
	enemy->setPositionText(enemy->getPosition());
	ScoreGame::getInstance()->setScore(ScoreGame::getInstance()->getScore() + 1000);
	SoundClass::getInstance()->playWaveFile(IDSounds::Sound_Stomp);
}

void BowserDieByGun::execute(Enemy* enemy)
{
	Vector2 velocity = enemy->getVelocity();
	velocity.y--;
	enemy->setVelocity(velocity);
	enemy->setTimeAnimation(enemy->getTimeAnimation() - 1);
	if (enemy->getTimeAnimation() <= 0)
		enemy->setActive(false);
}

void BowserDieByGun::exit(Enemy* enemy)
{
}


// Boss di chuyển 
BowserMove* BowserMove::m_instance = 0;

BowserMove::BowserMove()
{
	// <Xác định frame animation của Bowser>
	for (int i = 0; i < 2; i++)
		m_frameAnimation.push_back(Frame(720 + 32 * i, 0, 32, 32));
}

BowserMove* BowserMove::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new BowserMove();
	}

	return m_instance;
}

void BowserMove::enter(Enemy* enemy)
{
	enemy->setVelocity(Vector2(0, 0));
	enemy->setTimeAnimation(3);
	enemy->setFrameList(m_frameAnimation);
	enemy->setCurrentFrame(0);

	m_timeFire = 30;
}

void BowserMove::execute(Enemy* enemy)
{
	Vector2 marioPosition = Mario::getInstance()->getPosition();
	Vector2 enemyPosition = enemy->getPosition();

	// Xác định hướng đi cho boss
	if (enemy->getPosition().x > Mario::getInstance()->getPosition().x)
		enemy->setFliping(SpriteEffect::None);
	else if (enemy->getPosition().x < Mario::getInstance()->getPosition().x)
		enemy->setFliping(SpriteEffect::Flip);
	
	// Nếu khoảng cách giữa mario và boss < 100 thì boss sẽ tấn công
	if (abs(enemyPosition.x - marioPosition.x) < 100)
	{
		if (m_timeFire-- > 0) return;

		m_timeFire = 30;
		if (marioPosition.y > enemyPosition.y + enemy->getHeight())
			enemy->getStateMachine()->changeState(BowserAttackByHammer::getInstance());
		else
			enemy->getStateMachine()->changeState(BowserAttackByFire::getInstance());
		return;
	}

	// Nếu khoảng cách đạn của mario và boss < 100  thì sẽ nhảy lên
	for each (Bullet* bullet in Mario::getInstance()->getGun()->getBulletShooted())
	{
		Box enemyBound = enemy->getBouding();
		enemyBound.x -= 50;
		enemyBound.width += 50;  

		if (AABB(enemyBound, bullet->getBouding()) != DIR::NONE)
		{
			enemy->getStateMachine()->changeState(BowserJump::getInstance());
			return;
		}
	}
	// neu chuyen state
	if (enemy->getAttack() == BeAttack::DeathByGun)
	{
		enemy->getStateMachine()->changeState(BowserDieByGun::getInstance());
		return;
	}
		

	// Cập nhật van tốc
	Vector2 velocity = Vector2(1, 3 * GRAVITATION);
	if (enemy->getFliping() == SpriteEffect::None)
		velocity.x *= -1;
	enemy->setVelocity(velocity);


	// Animation
	int current = enemy->getCurrentFrame() + 1;
	if (current >= enemy->getSizeFrameList())//cập nhật indext
		current = 0;
	int time = enemy->getTimeAnimation() - 1;
	if (time <= 0)
	{
		enemy->setCurrentFrame(current);
		time = 3;
	}
	enemy->setTimeAnimation(time);
}

void BowserMove::exit(Enemy* enemy)
{
}


// Attack
BowserAttackByFire* BowserAttackByFire::m_instance = 0;

BowserAttackByFire::BowserAttackByFire()
{
	// <Xác định frame animation của Bowser>
	for (int i = 0; i < 2; i++)
		m_frameAnimation.push_back(Frame(656 + 32 * i, 0, 32, 32));
}

BowserAttackByFire* BowserAttackByFire::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new BowserAttackByFire();
	}

	return m_instance;
}

void BowserAttackByFire::enter(Enemy* enemy)
{
	enemy->setVelocity(Vector2(0, 0));
	enemy->setTimeAnimation(5);
	enemy->setFrameList(m_frameAnimation);
	enemy->setCurrentFrame(0);
}

void BowserAttackByFire::execute(Enemy* enemy)
{
	Vector2 marioPosition = Mario::getInstance()->getPosition();
	Vector2 enemyPosition = enemy->getPosition();
	// Xác định hướng
	if (marioPosition.x > enemyPosition.x)
		enemy->setFliping(SpriteEffect::Flip);
	else
		enemy->setFliping(SpriteEffect::None);

	Bowser* bowser = dynamic_cast<Bowser*>(enemy);
	if (bowser != nullptr)
	{
		// xét tọa độ đạn hợp lý
		if (enemy->getFliping() == SpriteEffect::Flip)
			enemyPosition.x += enemy->getWidth();
		else
			enemyPosition.x -= 15;
		Vector2 camera = Camera::getInstance()->getViewport();

		// Chỉ bắn khi viên hammer đã mất
		if (bowser->getManageHammer()->getBulletShooted().size() == 0)
			bowser->getManageBulletFire()->shoot(enemyPosition.x, enemyPosition.y + 15, camera.x, camera.y, enemy->getFliping() == SpriteEffect::Flip ? SpriteEffect::None : SpriteEffect::Flip);
	}

	// Cập nhật state
	if (enemy->getAttack() == BeAttack::DeathByGun)
	{
		enemy->getStateMachine()->changeState(BowserDieByGun::getInstance());
		return;
	}
	// Nếu khoảng cách đạn của mario và boss < 100  thì sẽ nhảy lên
	for each (Bullet* bullet in Mario::getInstance()->getGun()->getBulletShooted())
	{
		Box enemyBound = enemy->getBouding();
		enemyBound.x -= 50;
		enemyBound.width += 50;

		if (AABB(enemyBound, bullet->getBouding()) != DIR::NONE)
		{
			enemy->getStateMachine()->changeState(BowserJump::getInstance());
			return;
		}
	}

	// Animation
	int current = enemy->getCurrentFrame() + 1;
	if (current >= enemy->getSizeFrameList())//cập nhật indext
	{
		enemy->getStateMachine()->changeState(BowserMove::getInstance());
		return;
	}
		
	int time = enemy->getTimeAnimation() - 1;
	if (time <= 0)
	{
		enemy->setCurrentFrame(current);
		time = 5;
	}
	enemy->setTimeAnimation(time);
}

void BowserAttackByFire::exit(Enemy* enemy)
{
}


// Attack by hammer
BowserAttackByHammer* BowserAttackByHammer::m_instance = 0;

BowserAttackByHammer::BowserAttackByHammer()
{
	// <Xác định frame animation của Bowser>
	for (int i = 0; i < 2; i++)
		m_frameAnimation.push_back(Frame(656 + 32 * i, 0, 32, 32));
}

BowserAttackByHammer* BowserAttackByHammer::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new BowserAttackByHammer();
	}

	return m_instance;
}

void BowserAttackByHammer::enter(Enemy* enemy)
{
	enemy->setVelocity(Vector2(0, 0));
	enemy->setTimeAnimation(5);
	enemy->setFrameList(m_frameAnimation);
	enemy->setCurrentFrame(0);
}

void BowserAttackByHammer::execute(Enemy* enemy)
{
	Vector2 marioPosition = Mario::getInstance()->getPosition();
	Vector2 enemyPosition = enemy->getPosition();
	// Xác định hướng
	if (marioPosition.x > enemyPosition.x)
		enemy->setFliping(SpriteEffect::Flip);
	else 
		enemy->setFliping(SpriteEffect::None);

	Bowser* bowser = dynamic_cast<Bowser*>(enemy);
	if (bowser != nullptr)
	{
		// xét tọa độ đạn hợp lý
		if (enemy->getFliping() == SpriteEffect::Flip)
			enemyPosition.x += enemy->getWidth();
		Vector2 camera = Camera::getInstance()->getViewport();
		// Chỉ bắn khi viên đạn lửa đã mất
		if (bowser->getManageBulletFire()->getBulletShooted().size() == 0)
			bowser->getManageHammer()->shoot(enemyPosition.x, enemyPosition.y + enemy->getHeight(), camera.x, camera.y, enemy->getFliping() == SpriteEffect::Flip ? SpriteEffect::None : SpriteEffect::Flip);
	}


	// Cập nhật state
	if (enemy->getAttack() == BeAttack::DeathByGun)
	{
		enemy->getStateMachine()->changeState(BowserDieByGun::getInstance());
		return;
	}
	// Nếu khoảng cách đạn của mario và boss < 100  thì sẽ nhảy lên
	for each (Bullet* bullet in Mario::getInstance()->getGun()->getBulletShooted())
	{
		Box enemyBound = enemy->getBouding();
		enemyBound.x -= 50;
		enemyBound.width += 50;

		if (AABB(enemyBound, bullet->getBouding()) != DIR::NONE)
		{
			enemy->getStateMachine()->changeState(BowserJump::getInstance());
			return;
		}
	}

	// Animation
	int current = enemy->getCurrentFrame() + 1;
	if (current >= enemy->getSizeFrameList())//cập nhật indext
	{
		enemy->getStateMachine()->changeState(BowserMove::getInstance());
		return;
	}
	int time = enemy->getTimeAnimation() - 1;
	if (time <= 0)
	{
		enemy->setCurrentFrame(current);
		time = 5;
	}
	enemy->setTimeAnimation(time);
}

void BowserAttackByHammer::exit(Enemy* enemy)
{
}


// Jump
BowserJump* BowserJump::m_instance = 0;

BowserJump* BowserJump::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new BowserJump();
	}

	return m_instance;
}

void BowserJump::enter(Enemy* enemy)
{
	enemy->setVelocity(Vector2(0, 8));
	enemy->setCurrentFrame(0);
	enemy->setLocation(Location::LOC_IN_AIR);
}

void BowserJump::execute(Enemy* enemy)
{
	Vector2 velocity = enemy->getVelocity();
	velocity.y--;
	enemy->setVelocity(velocity);
	if (enemy->getLocation() == Location::LOC_ON_GROUND)
		enemy->getStateMachine()->changeState(BowserMove::getInstance());
}

void BowserJump::exit(Enemy* enemy)
{
}
