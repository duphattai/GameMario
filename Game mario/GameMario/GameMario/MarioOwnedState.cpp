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
	mario->setFSM(FSM_Mario::STAND);
	mario->setVelocity(Vector2(0, 0));
}

void Standing::execute(Mario* mario)
{
	if (mario->isBig())
		mario->setCurrentFrame(MarioSheet::BIG_MARIO_STAND);
	else if (mario->canShoot())
		mario->setCurrentFrame(MarioSheet::SUPER_MARIO_STAND);
	else
		mario->setCurrentFrame(MarioSheet::MARIO_STAND);


	mario->setVelocity(Vector2(0, GRAVITATION));

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
	else if (keyboard->isKeyDown(DIK_DOWN) && (mario->isBig() || mario->canShoot()))
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

	if (mario->isBig())
		mario->setCurrentFrame(MarioSheet::BIG_MARIO_RUN);
	else if (mario->canShoot())
		mario->setCurrentFrame(MarioSheet::SUPER_MARIO_RUN);
	else
		mario->setCurrentFrame(MarioSheet::MARIO_RUN);

	mario->setFSM(FSM_Mario::RUN);
}

void Running::execute(Mario* mario)
{
	Vector2 velocity = mario->getVelocity();
	velocity.y = GRAVITATION; // gravity


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
		mario->getStateMachine()->changeState(Sitting::getInstance());
	}
	else if(keyboard->isKeyDown(DIK_RIGHT))
	{
		mario->setFliping(SpriteEffect::None);
		mario->setCurrentFrame(index);
		velocity.x += 0.50f;
	}
	else if(keyboard->isKeyDown(DIK_LEFT))
	{
		mario->setFliping(SpriteEffect::Flip);
		mario->setCurrentFrame(index);
		velocity.x -= 0.50f;
	}
	else // không ấn phím, trả về standing
	{
		// make accel
		mario->setCurrentFrame(index);
		if (mario->getFliping() == SpriteEffect::Flip) // move left
		{
			velocity.x = (velocity.x + 0.5f) >= 0 ? 0 : (velocity.x + 0.5f);
		}
		else // move right;
			velocity.x = (velocity.x - 0.5f) <= 0 ? 0 : (velocity.x - 0.5f);	

		if (velocity.x == 0 && mario->getLocation() == Location::LOC_ON_GROUND)
			mario->getStateMachine()->changeState(Standing::getInstance());
	}
	mario->setVelocity(velocity);


	if (keyboard->isKeyDown(DIK_UP))
		mario->getStateMachine()->changeState(Jumping::getInstance());
	else if (mario->getLocation() == Location::LOC_IN_AIR)
		mario->getStateMachine()->changeState(Falling::getInstance());
}

void Running::exit(Mario* mario)
{
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
	if (mario->isBig())
		mario->setCurrentFrame(MarioSheet::BIG_MARIO_SIT);
	else if (mario->canShoot())
		mario->setCurrentFrame(MarioSheet::SUPER_MARIO_SIT);

	mario->setFSM(FSM_Mario::SIT);
}

void Sitting::execute(Mario* mario)
{
	// make accel
	Vector2 velocity = mario->getVelocity();
	if (mario->getFliping() == Flip) // left
	{
		if (velocity.x < 0)
			velocity.x++;
	}
	else
	{
		if (velocity.x > 0)
			velocity.x--;
	}
	mario->setVelocity(velocity);



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
	if (mario->isBig())
		mario->setCurrentFrame(MarioSheet::BIG_MARIO_STAND);
	else if (mario->canShoot())
		mario->setCurrentFrame(MarioSheet::SUPER_MARIO_STAND);
	else
		mario->setCurrentFrame(MarioSheet::MARIO_STAND);

	mario->setLocation(Location::LOC_IN_AIR);
	mario->setVelocity(Vector2(mario->getVelocity().x, 6.0f));
	m_timeJump = 4;
	mario->setFSM(FSM_Mario::JUMP);
}

void Jumping::execute(Mario* mario)
{
	// mặc định load sprite jump
	if (mario->isBig())
		mario->setCurrentFrame(MarioSheet::BIG_MARIO_JUMP);
	else if (mario->canShoot())
		mario->setCurrentFrame(MarioSheet::SUPER_MARIO_JUMP);
	else
		mario->setCurrentFrame(MarioSheet::MARIO_JUMP);


	Vector2 velocity = mario->getVelocity();
	

	// solve if has coliision
	DIR dir = mario->getDirCollision();
	if (dir != DIR::NONE) // iscollision
	{
		if (dir != DIR::TOP) // left or right
		{
			mario->getStateMachine()->changeState(Falling::getInstance());
		}
		else if (velocity.y <= 0 || dir == DIR::TOP)
		{
			mario->getStateMachine()->changeState(Standing::getInstance());
		}
		return;
	}


	velocity.y += GRAVITATION;
	// update velocity
	keyboard->getState();
	if (keyboard->isKeyDown(DIK_UP) && m_timeJump-- > 0)
		velocity.y += m_timeJump;

	if (keyboard->isKeyDown(DIK_RIGHT))
	{
		mario->setFliping(SpriteEffect::None);
		// không cho nhảy xa quá
		velocity.x += 0.50f;
	}
	else if (keyboard->isKeyDown(DIK_LEFT))
	{
		mario->setFliping(SpriteEffect::Flip);
		// không cho nhảy xa quá
		velocity.x -= 0.50f;
	}
	else if (keyboard->isKeyDown(DIK_DOWN) && (mario->isBig() || mario->canShoot()))
	{
		if (mario->isBig())
			mario->setCurrentFrame(MarioSheet::BIG_MARIO_SIT);
		else
			mario->setCurrentFrame(MarioSheet::SUPER_MARIO_SIT);
	}
	mario->setVelocity(velocity);
}

void Jumping::exit(Mario* mario)
{
	m_timeJump = 4;
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
	mario->setFSM(FSM_Mario::FALL);
}

void Falling::execute(Mario* mario)
{
	// mặc định load sprite jump
	if (mario->isBig())
		mario->setCurrentFrame(MarioSheet::BIG_MARIO_JUMP);
	else if (mario->canShoot())
		mario->setCurrentFrame(MarioSheet::SUPER_MARIO_JUMP);
	else
		mario->setCurrentFrame(MarioSheet::MARIO_JUMP);

	Vector2 velocity = mario->getVelocity();
	velocity.y += GRAVITATION;


	// update velocity
	keyboard->getState();
	if (keyboard->isKeyDown(DIK_RIGHT))
	{
		mario->setFliping(SpriteEffect::None);
		velocity.x += 0.50f;
	}
	else if (keyboard->isKeyDown(DIK_LEFT))
	{
		mario->setFliping(SpriteEffect::Flip);
		velocity.x -= 0.50f;
	}
	else if (keyboard->isKeyDown(DIK_DOWN) && (mario->isBig() || mario->canShoot()))
	{
		if (mario->isBig())
			mario->setCurrentFrame(MarioSheet::BIG_MARIO_SIT);
		else
			mario->setCurrentFrame(MarioSheet::SUPER_MARIO_SIT);
	}
	mario->setVelocity(velocity);

	//// nếu xảy ra va chạm
	if (mario->getLocation() == Location::LOC_ON_GROUND)
		mario->getStateMachine()->changeState(Standing::getInstance());
}

void Falling::exit(Mario* mario)
{
}
/////////////////////////////////////////////////////////////////////////////////

