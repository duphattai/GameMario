#include "Mario.h"
#include "MarioOwnedState.h"
#include "KeyBoard.h"

/////////////////////////////////////////////////////////////////////////////////
//STANDING
Standing* Standing::m_instance = 0;
extern CKeyBoard *keyboard; // ý nghĩa là keyboard đã được đinh nghĩa ở đâu đó
Standing* Standing::getInstance()
{
	if (!m_instance)
		m_instance = new Standing();

	return m_instance;
}

void Standing::enter(Mario* mario)
{
	if (mario->isBig())
		mario->setCurrentFrame(MarioSheet::BIG_MARIO_STAND);
	else if (mario->canShoot())
		mario->setCurrentFrame(MarioSheet::SUPER_MARIO_STAND);
	else
		mario->setCurrentFrame(MarioSheet::MARIO_STAND);

	mario->setLocation(Location::LOC_ON_GROUND);
	//Coder: Tai
	//mario->setWorldPosition(mario->getWorldPosition().x, mario->getPosition().y);
}

void Standing::execute(Mario* mario)
{
	mario->setVelocity(Vector2(0, 0));


	keyboard->getState();
	if (keyboard->isKeyDown(DIK_LEFT))
	{
		mario->setFliping(SpriteEffect::Flip);
		mario->getStateMachine()->changeState(Running::getInstance());
	}	
	else if (keyboard->isKeyDown(DIK_RIGHT))
	{
		mario->setFliping(SpriteEffect::None);
		mario->getStateMachine()->changeState(Running::getInstance());
	}
	else if (keyboard->isKeyDown(DIK_DOWN) && mario->isBig())
		mario->getStateMachine()->changeState(Sitting::getInstance());
	else if (keyboard->isKeyDown(DIK_UP))
		mario->getStateMachine()->changeState(Jumping::getInstance());
	
}

void Standing::exit(Mario* mario)
{

}
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//RUNNING
Running* Running::m_instance = 0;

Running* Running::getInstance()
{
	if (!m_instance)
		m_instance = new Running();

	return m_instance;
}

void Running::enter(Mario* mario)
{
	mario->setLocation(Location::LOC_ON_GROUND);

	if (!mario->isBig())
		mario->setCurrentFrame(MarioSheet::MARIO_RUN);
	else if (!mario->canShoot())
		mario->setCurrentFrame(MarioSheet::BIG_MARIO_RUN);
	else
		mario->setCurrentFrame(MarioSheet::SUPER_MARIO_RUN);

	m_accel = 4; // make acell
}

void Running::execute(Mario* mario)
{
	Vector2 velocity = mario->getVelocity();

	// update animation of mario
	int index = mario->getCurrentFrame() + 1;
	if (mario->isBig())
		index = index >= MarioSheet::BIG_MARIO_CHANGE_DIR ? (MarioSheet::BIG_MARIO_RUN) : index; // cap nhật lại index big mario
	else if (mario->canShoot())
		index = index >= MarioSheet::SUPER_MARIO_CHANGE_DIR ? (MarioSheet::SUPER_MARIO_RUN) : index; // cap nhật lại index fire mario
	else
		index = index >= MarioSheet::MARIO_CHANGE_DIR ? (MarioSheet::MARIO_RUN) : index; // cap nhật lại index small mario


	// update velocity
	keyboard->getState();
	if (keyboard->isKeyDown(DIK_DOWN) && (mario->isBig() || mario->canShoot()))
	{
		velocity.x /= abs(velocity.x);
		mario->getStateMachine()->changeState(Sitting::getInstance());
	}
	else if(keyboard->isKeyDown(DIK_RIGHT))
	{
		mario->setFliping(SpriteEffect::None);
		mario->setCurrentFrame(index);
		velocity.x++;
	}
	else if(keyboard->isKeyDown(DIK_LEFT))
	{
		mario->setFliping(SpriteEffect::Flip);
		mario->setCurrentFrame(index);
		velocity.x--;
	}
	else // không ấn phím, trả về standing
	{
		m_accel--;
		mario->setCurrentFrame(index);
		if (m_accel == 0)
			mario->getStateMachine()->changeState(Standing::getInstance());
	}
	mario->setVelocity(velocity);

	if (keyboard->isKeyDown(DIK_UP))
	{
		mario->getStateMachine()->changeState(Jumping::getInstance());
	}
}

void Running::exit(Mario* mario)
{
	m_accel = 4;
}
/////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////
//SITTING
Sitting* Sitting::m_instance = 0;

Sitting* Sitting::getInstance()
{
	if (!m_instance)
		m_instance = new Sitting();

	return m_instance;
}

void Sitting::enter(Mario* mario)
{
	mario->setCurrentFrame(MarioSheet::BIG_MARIO_SIT);

	m_accel = 4;
	Vector2 velocity = mario->getVelocity();
	velocity.x = 0;
	mario->setVelocity(velocity);
}

void Sitting::execute(Mario* mario)
{
	m_accel--;
	if (count == 0) // make accel
	{
		Vector2 velocity = mario->getVelocity();
		velocity.x = 0;
		mario->setVelocity(velocity);
	}

	keyboard->getState();
	if (keyboard->isKeyDown(DIK_UP))
	{
		mario->getStateMachine()->changeState(Jumping::getInstance());
	}
	else if (!keyboard->isKeyDown(DIK_DOWN))
	{
		mario->getStateMachine()->changeState(Standing::getInstance());
	}
}

void Sitting::exit(Mario* mario)
{
	m_accel = 4;
}
/////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////
//JUMPING
Jumping* Jumping::m_instance = 0;

Jumping* Jumping::getInstance()
{
	if (!m_instance)
		m_instance = new Jumping();

	return m_instance;
}

void Jumping::enter(Mario* mario)
{
	// mặc định load sprite jump
	if (!mario->isBig())
		mario->setCurrentFrame(MarioSheet::MARIO_JUMP);
	else if (!mario->canShoot())
		mario->setCurrentFrame(MarioSheet::BIG_MARIO_JUMP);
	else
		mario->setCurrentFrame(MarioSheet::SUPER_MARIO_JUMP);

	mario->setLocation(Location::LOC_IN_AIR);

	mario->setVelocity(Vector2(mario->getVelocity().x, mario->getMaxVelocity().y));
}

void Jumping::execute(Mario* mario)
{
	// mặc định load sprite jump
	if (!mario->isBig())
		mario->setCurrentFrame(MarioSheet::MARIO_JUMP);
	else if (!mario->canShoot())
		mario->setCurrentFrame(MarioSheet::BIG_MARIO_JUMP);
	else
		mario->setCurrentFrame(MarioSheet::SUPER_MARIO_JUMP);

	Vector2 velocity = mario->getVelocity();
	Vector2 maxVelocity = mario->getMaxVelocity();
	Vector2 minVelocity = mario->getMinVelocity();
	velocity.y -= 2;

	// update velocity
	keyboard->getState();
	if (keyboard->isKeyDown(DIK_RIGHT))
	{
		mario->setFliping(SpriteEffect::None);
		// không cho nhảy xa quá
		velocity.x = (++velocity.x) >= (maxVelocity.x / 2) ? (maxVelocity.x / 2) : velocity.x; // cần thiết
	}
	else if (keyboard->isKeyDown(DIK_LEFT))
	{
		mario->setFliping(SpriteEffect::Flip);
		// không cho nhảy xa quá
		velocity.x = (--velocity.x) <= (minVelocity.x / 2) ? (minVelocity.x / 2) : velocity.x; // cần thiết
	}
	else if (keyboard->isKeyDown(DIK_DOWN) && (mario->isBig() || mario->canShoot()))
	{
		if (mario->isBig())
			mario->setCurrentFrame(MarioSheet::BIG_MARIO_SIT);
		else
			mario->setCurrentFrame(MarioSheet::SUPER_MARIO_SIT);
	}
	mario->setVelocity(velocity);

	if (velocity.y <= 0)
	{
		mario->getStateMachine()->changeState(Falling::getInstance());
	}
}

void Jumping::exit(Mario* mario)
{
}
/////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////
//FALLING
Falling* Falling::m_instance = 0;

Falling* Falling::getInstance()
{
	if (!m_instance)
		m_instance = new Falling();

	return m_instance;
}

void Falling::enter(Mario* mario)
{
	mario->setLocation(Location::LOC_IN_AIR);
	mario->setVelocity(Vector2(mario->getVelocity().x, 0));
}

void Falling::execute(Mario* mario)
{
	// mặc định load sprite jump
	if (!mario->isBig())
		mario->setCurrentFrame(MarioSheet::MARIO_JUMP);
	else if (!mario->canShoot())
		mario->setCurrentFrame(MarioSheet::BIG_MARIO_JUMP);
	else
		mario->setCurrentFrame(MarioSheet::SUPER_MARIO_JUMP);

	Vector2 velocity = mario->getVelocity();
	Vector2 minVelocity = mario->getMinVelocity();
	Vector2 maxVelocity = mario->getMaxVelocity();
	velocity.y -= 2;

	// update velocity
	keyboard->getState();
	if (keyboard->isKeyDown(DIK_RIGHT))
	{
		mario->setFliping(SpriteEffect::None);
		velocity.x = (++velocity.x) >= (maxVelocity.x / 2) ? (maxVelocity.x / 2) : velocity.x; // cần thiết
	}
	else if (keyboard->isKeyDown(DIK_LEFT))
	{
		mario->setFliping(SpriteEffect::Flip);
		velocity.x = (--velocity.x) <= (minVelocity.x / 2) ? (minVelocity.x / 2) : velocity.x; // cần thiết
	}
	else if (keyboard->isKeyDown(DIK_DOWN) && (mario->isBig() || mario->canShoot()))
	{
		if (mario->isBig())
			mario->setCurrentFrame(MarioSheet::BIG_MARIO_SIT);
		else
			mario->setCurrentFrame(MarioSheet::SUPER_MARIO_SIT);
	}
	else{}
	mario->setVelocity(velocity);

	//// nếu xảy ra va chạm
	//if (mario->getLocation() == Location::LOC_ON_GROUND)
	//	mario->getStateMachine()->changeState(Standing::getInstance());
	if (mario->getPosition().y <= 32)
	{
		mario->getStateMachine()->changeState(Standing::getInstance());
	}
}

void Falling::exit(Mario* mario)
{
	mario->setLocation(Location::LOC_ON_GROUND);
}
/////////////////////////////////////////////////////////////////////////////////

