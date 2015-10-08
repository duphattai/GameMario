#include "Mario.h"
#include "MarioOwnedState.h"
#include "KeyBoard.h"

extern CKeyBoard *keyboard; // ý nghĩa là keyboard đã được đinh nghĩa ở đâu đó


// action of mario
#pragma region
/////////////////////////////////////////////////////////////////////////////////
//STANDING
Standing* Standing::m_instance = 0;

Standing* Standing::getInstance()
{
	if (!m_instance)
		m_instance = new Standing();

	return m_instance;
}

void Standing::enter(Mario* mario)
{
	mario->setLocation(Location::LOC_ON_GROUND);
	mario->setFSM(FSM_Mario::STAND);
	mario->setVelocity(Vector2(0, 0));
}

void Standing::execute(Mario* mario)
{
	mario->setVelocity(Vector2(0, GRAVITATION));

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
	mario->setFSM(FSM_Mario::RUN);
}

void Running::execute(Mario* mario)
{
	Vector2 velocity = mario->getVelocity();
	velocity.y = GRAVITATION; // gravity

	// update velocity
	if (keyboard->isKeyDown(DIK_DOWN) && (mario->isBig() || mario->canShoot()))
	{
		mario->getStateMachine()->changeState(Sitting::getInstance());
		return;
	}
	else if(keyboard->isKeyDown(DIK_RIGHT))
	{		
		mario->setFliping(SpriteEffect::None);
		velocity.x += 0.50f;
	}
	else if(keyboard->isKeyDown(DIK_LEFT))
	{
		mario->setFliping(SpriteEffect::Flip);
		velocity.x -= 0.50f;
	}
	else // không ấn phím, trả về standing
	{
		// make accel
		if (mario->getFliping() == SpriteEffect::Flip) // move left
			velocity.x = (velocity.x + 0.5f) >= 0 ? 0 : (velocity.x + 0.5f);
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
	mario->setFSM(FSM_Mario::SIT);
}

void Sitting::execute(Mario* mario)
{
	// make accel
	Vector2 velocity = mario->getVelocity();
	if (mario->getFliping() == Flip && velocity.x < 0) // left
		velocity.x++;
	else if (mario->getFliping() == SpriteEffect::None && velocity.x > 0)
		velocity.x--;
	mario->setVelocity(velocity);


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
	mario->setLocation(Location::LOC_IN_AIR);
	mario->setVelocity(Vector2(mario->getVelocity().x, 6.00f));
	m_timeJump = 4;
	mario->setFSM(FSM_Mario::JUMP);
}

void Jumping::execute(Mario* mario)
{
	Vector2 velocity = mario->getVelocity();	

	// solve if has coliision
	DIR dir = mario->getDirCollision();
	if (dir != DIR::NONE) // iscollision
	{
		if (dir != DIR::TOP) // left or right
			mario->getStateMachine()->changeState(Falling::getInstance());
		else if (velocity.y <= 0 || dir == DIR::TOP)
			mario->getStateMachine()->changeState(Standing::getInstance());
		return;
	}


	velocity.y += GRAVITATION;
	// update velocity
	if (keyboard->isKeyDown(DIK_UP) && --m_timeJump > 0)
		velocity.y += m_timeJump;

	if (keyboard->isKeyDown(DIK_RIGHT))
	{
		mario->setFliping(SpriteEffect::None);
		// không cho nhảy xa quá
		velocity.x += 1.0f;
	}
	else if (keyboard->isKeyDown(DIK_LEFT))
	{
		mario->setFliping(SpriteEffect::Flip);
		// không cho nhảy xa quá
		velocity.x -= 1.00f;
	}
	else if (keyboard->isKeyDown(DIK_DOWN) && (mario->isBig() || mario->canShoot()))
	{
		if (mario->isBig())
			mario->setCurrentFrame(MarioSheet::BIG_MARIO_SIT);
		else
			mario->setCurrentFrame(MarioSheet::BIG_SUPER_SIT);
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
	Vector2 velocity = mario->getVelocity();
	velocity.y += GRAVITATION;


	// update velocity
	if (keyboard->isKeyDown(DIK_RIGHT))
	{
		mario->setFliping(SpriteEffect::None);
		velocity.x += 1.00f;
	}
	else if (keyboard->isKeyDown(DIK_LEFT))
	{
		mario->setFliping(SpriteEffect::Flip);
		velocity.x -= 1.00f;
	}
	else if (keyboard->isKeyDown(DIK_DOWN) && (mario->isBig() || mario->canShoot()))
	{
		if (mario->isBig())
			mario->setCurrentFrame(MarioSheet::BIG_MARIO_SIT);
		else
			mario->setCurrentFrame(MarioSheet::BIG_SUPER_SIT);
	}
	mario->setVelocity(velocity);

	//// nếu xảy ra va chạm
	if (mario->getLocation() == Location::LOC_ON_GROUND)
	{
		if (keyboard->isKeyDown(DIK_LEFT) || keyboard->isKeyDown(DIK_RIGHT))
			mario->getStateMachine()->changeState(Running::getInstance());
		else
			mario->getStateMachine()->changeState(Standing::getInstance());
	}
		
}

void Falling::exit(Mario* mario)
{
}
/////////////////////////////////////////////////////////////////////////////////

#pragma endregion


// status of mario
#pragma region
/////////////////////////////////////////////////////////////////////////////////
// small
Small* Small::m_instance = 0;

Small::Small()
{
}

Small* Small::getInstance()
{
	if (!m_instance)
		m_instance = new Small();

	return m_instance;
}

void Small::enter(Mario* mario)
{
	mario->setIsBig(false);
	mario->setCanShoot(false);
	mario->m_effectSmall = true;

	int bigIndex = mario->getCurrentFrame();
	bigIndex = bigIndex > MarioSheet::BIG_MARIO_SIT ? bigIndex - 15 : bigIndex; // nếu index > index cua big mario thi chuyen index ve big mario

	// make effect mario big -> small
	m_frameAnimation.push_back(bigIndex); // big mario
	m_frameAnimation.push_back(bigIndex + 8); // fire mario
	m_frameAnimation.push_back(bigIndex); // invinbility color 1
	m_frameAnimation.push_back(bigIndex + 22); // invinbility color 2
	m_frameAnimation.push_back(bigIndex); // invinbility color 1
	m_frameAnimation.push_back(bigIndex + 8); // fire mario

	// make effect after small
	m_frameAnimationAfterEffectSmall.push_back(0); // current index
	//m_frameAnimationAfterEffectSmall.push_back(26-8); // small fire mario
	m_frameAnimationAfterEffectSmall.push_back(39 - 8); // small invinbility color 1
	//m_frameAnimationAfterEffectSmall.push_back(26 - 8); // small fire mario
	m_frameAnimationAfterEffectSmall.push_back(0); // current index

	m_timeChangeSprite = 1;
}

void Small::execute(Mario* mario)
{
	if (mario->m_effectSmall)
	{
		if (m_currentIndex == m_frameAnimation.size())
		{	
			mario->m_effectSmall = false;
			m_countTime = 30;
			m_currentIndex = 0;
		}
		else if (m_timeChangeSprite-- == 0)
		{
			mario->setCurrentFrame(m_frameAnimation[m_currentIndex++]);
			m_timeChangeSprite = 1;
		}		
	}
	else
	{
		//lấy index của ảnh trước khi cập nhật effect after small
		int index;
		if (m_currentIndex == 0)
			mario->setCurrentFrame(mario->getCurrentFrame() - m_frameAnimationAfterEffectSmall[m_frameAnimationAfterEffectSmall.size() - 1]);
		else
			mario->setCurrentFrame(mario->getCurrentFrame() - m_frameAnimationAfterEffectSmall[(m_currentIndex - 1)]);


		// update animation of mario
		if (mario->getFSM() == FSM_Mario::RUN)
		{
			int index = mario->getCurrentFrame() + 1;
			// cap nhật lại index small mario
			if (index >= MarioSheet::MARIO_CHANGE_DIR || index < MarioSheet::MARIO_RUN)
				index = MarioSheet::MARIO_RUN;

			mario->setCurrentFrame(index);
		}
		else if (mario->getFSM() == FSM_Mario::STAND)
			mario->setCurrentFrame(MarioSheet::MARIO_STAND);
		else if (mario->getFSM() == FSM_Mario::JUMP || mario->getFSM() == FSM_Mario::FALL)
			mario->setCurrentFrame(MarioSheet::MARIO_JUMP);

		// update mờ ảnh sau khi thực hiện smaller
		if (--m_countTime > 0)
		{
			mario->setCurrentFrame(mario->getCurrentFrame() + m_frameAnimationAfterEffectSmall[m_currentIndex++]);
			if (m_currentIndex >= m_frameAnimationAfterEffectSmall.size())
				m_currentIndex = 0;
		}
		else
			m_currentIndex = 0;


		if (mario->isBig())
		{
			mario->getStatusStateMachine()->changeState(Big::getInstance());
		}
		else if (mario->isDead()) // không gọi lại
		{
			mario->getStatusStateMachine()->changeState(Dead::getInstance());
		}
		else if (mario->isStar())
			mario->getStatusStateMachine()->changeState(Star::getInstance());
	}
}

void Small::exit(Mario* mario)
{
	m_frameAnimation.clear();
	m_currentIndex = 0;
	m_countTime = 0;
}

/////////////////////////////////////////////////////////////////////////////////
//Big
Big* Big::m_instance = 0;

Big::Big()
{
	m_frameAnimation.push_back(MarioSheet::MARIO_STAND);
	m_frameAnimation.push_back(MarioSheet::BIG_SMALL_STAND);
	m_frameAnimation.push_back(MarioSheet::MARIO_STAND);
	m_frameAnimation.push_back(MarioSheet::BIG_MARIO_STAND);
	m_frameAnimation.push_back(MarioSheet::BIG_SMALL_STAND);
	m_frameAnimation.push_back(MarioSheet::BIG_MARIO_STAND);
}

Big* Big::getInstance()
{
	if (!m_instance)
		m_instance = new Big();

	return m_instance;
}

void Big::enter(Mario* mario)
{
	mario->setIsBig(true);
	mario->setCanShoot(false);

	mario->m_effectBig = true;
}

void Big::execute(Mario* mario)
{
	if (mario->m_effectBig)
	{
		if (m_currentIndex == m_frameAnimation.size())
		{
			mario->m_effectBig = false;
		}
		else
			mario->setCurrentFrame(m_frameAnimation[m_currentIndex++]);
	}
	else
	{
		// update animation of mario
		if (mario->getFSM() == FSM_Mario::RUN)
		{
			int index = mario->getCurrentFrame() + 1;
			// cap nhật lại index big mario
			if (index >= MarioSheet::BIG_MARIO_CHANGE_DIR || index < MarioSheet::BIG_MARIO_RUN)
				index = BIG_MARIO_RUN;

			mario->setCurrentFrame(index);
		}
		else if (mario->getFSM() == FSM_Mario::STAND)
			mario->setCurrentFrame(MarioSheet::BIG_MARIO_STAND);
		else if (mario->getFSM() == FSM_Mario::SIT)
			mario->setCurrentFrame(MarioSheet::BIG_MARIO_SIT);
		else if (mario->getFSM() == FSM_Mario::FALL || mario->getFSM() == FSM_Mario::JUMP)
		{
			if (keyboard->getKeyCode() == DIK_DOWN)
				mario->setCurrentFrame(MarioSheet::BIG_MARIO_SIT);
			else
				mario->setCurrentFrame(MarioSheet::BIG_MARIO_JUMP);
		}



		if (mario->canShoot())
		{
			mario->getStatusStateMachine()->changeState(Fire::getInstance());
		}
		else if (mario->isDead())
		{
			mario->getStatusStateMachine()->changeState(Small::getInstance());
		}
		else if (mario->isStar())
		{
			mario->getStatusStateMachine()->changeState(Star::getInstance());
		}
	}
	
}

void Big::exit(Mario* mario)
{
	mario->setDead(false);// cập nhật lại
	m_currentIndex = 0;
}

// Dead
Dead* Dead::m_instance = 0;

Dead::Dead()
{
}

Dead* Dead::getInstance()
{
	if (!m_instance)
		m_instance = new Dead();

	return m_instance;
}

void Dead::enter(Mario* mario)
{
	mario->setVelocity(Vector2(0, 5));
	mario->setCurrentFrame(MarioSheet::MARIO_DIE);
	mario->setFSM(FSM_Mario::DEAD);

	mario->setIsBig(false);
	mario->setCanShoot(false);
}

void Dead::execute(Mario* mario)
{
	Vector2 velocity = mario->getVelocity();
	velocity.x = 0;
	velocity.y--;
	mario->setVelocity(velocity);


	if (mario->getLives() > 0 && mario->getPosition().y < -10)
	{
		Vector2 position = mario->getWorldPosition();
		mario->setPosition(position.x, 50);

		mario->getStatusStateMachine()->changeState(Small::getInstance());
		mario->getStateMachine()->changeState(Falling::getInstance());


		mario->m_effectSmall = false;
	}
}

void Dead::exit(Mario* mario)
{
	mario->setFSM(FSM_Mario::FALL);
	mario->setDead(false);
	mario->setIsBig(false);
	mario->setCanShoot(false);
	mario->setCurrentFrame(MarioSheet::MARIO_JUMP);
}

/////////////////////////////////////Fire
Fire* Fire::m_instance = 0;

Fire::Fire()
{
}

Fire* Fire::getInstance()
{
	if (!m_instance)
		m_instance = new Fire();

	return m_instance;
}

void Fire::enter(Mario* mario)
{
	mario->setIsBig(false);
	mario->setCanShoot(true);
	mario->m_effectFire = true;
	m_currentIndex = 0;

	int bigIndex = mario->getCurrentFrame();
	m_frameAnimation.push_back(bigIndex); // big mario
	m_frameAnimation.push_back(bigIndex + 15); // fire mario
	m_frameAnimation.push_back(bigIndex + 32); // invinbility color 1
	m_frameAnimation.push_back(bigIndex + 45); // invinbility color 2
	m_frameAnimation.push_back(bigIndex + 32); // invinbility color 1
	m_frameAnimation.push_back(bigIndex + 15); // fire mario
	m_frameAnimation.push_back(bigIndex + 32); // invinbility color 1
	m_frameAnimation.push_back(bigIndex + 15); // fire mario

	m_timeChangeSprite = 2;
}

void Fire::execute(Mario* mario)
{
	if (mario->m_effectFire)
	{
		if (m_currentIndex == m_frameAnimation.size())
		{
			mario->m_effectFire = false;
		}
		else if (--m_timeChangeSprite == 0)
		{
			mario->setCurrentFrame(m_frameAnimation[m_currentIndex++]);
			m_timeChangeSprite = 2;
		}	
	}
	else
	{
		if (mario->getFSM() == FSM_Mario::RUN)
		{
			int index = mario->getCurrentFrame() + 1;
			// cap nhật lại index fire mario
			if (index >= MarioSheet::BIG_SUPER_CHANGE_DIR || index < MarioSheet::BIG_SUPER_RUN)
				index = MarioSheet::BIG_SUPER_RUN;
			mario->setCurrentFrame(index);
		}
		else if (mario->getFSM() == FSM_Mario::STAND)
			mario->setCurrentFrame(MarioSheet::BIG_SUPER_STAND);
		else if (mario->getFSM() == FSM_Mario::SIT)
			mario->setCurrentFrame(MarioSheet::BIG_SUPER_SIT);
		else if (mario->getFSM() == FSM_Mario::FALL || mario->getFSM() == FSM_Mario::JUMP)
		{
			if (keyboard->getKeyCode() == DIK_DOWN)
				mario->setCurrentFrame(MarioSheet::BIG_SUPER_SIT);
			else
				mario->setCurrentFrame(MarioSheet::BIG_SUPER_JUMP);
		}


		if (mario->isDead())
			mario->getStatusStateMachine()->changeState(Small::getInstance());
	}
}

void Fire::exit(Mario* mario)
{
	mario->setDead(false);
	mario->setCanShoot(false);
	m_frameAnimation.clear();
}

///////////  efect start
Star* Star::m_instance = 0;

Star::Star()
{
	m_frameAnimationBig = new vector<int>();
	m_frameAnimationBig->push_back(0); // big mario
	m_frameAnimationBig->push_back(15); // fire mario
	m_frameAnimationBig->push_back(32); // invinbility color 1
	m_frameAnimationBig->push_back(45); // fire mario
	m_frameAnimationBig->push_back(32); // invinbility color 1
	m_frameAnimationBig->push_back(15); // fire mario
	m_frameAnimationBig->push_back(0); // big mario

	m_frameAnimationSmall = new vector<int>();
	m_frameAnimationSmall->push_back(0); // mario
	m_frameAnimationSmall->push_back(26 - 8); // fire mario
	m_frameAnimationSmall->push_back(39 - 8); // invinbility color 1
	m_frameAnimationSmall->push_back(52 - 8); // fire mario
	m_frameAnimationSmall->push_back(39 - 8); // invinbility color 1
	m_frameAnimationSmall->push_back(26 - 8); // fire mario
	m_frameAnimationSmall->push_back(0); // mario
}

Star* Star::getInstance()
{
	if (!m_instance)
		m_instance = new Star();

	return m_instance;
}

void Star::enter(Mario* mario)
{
	m_currentIndex = 0;
	m_timeCount = 150;
}

void Star::execute(Mario* mario)
{
	// update status of mario
	mario->setIsBig(mario->isBig());
	mario->setCanShoot(mario->canShoot());
	if (mario->isBig() || mario->canShoot())
		m_frameAnimation = m_frameAnimationBig;
	else // for small
		m_frameAnimation = m_frameAnimationSmall;


	// lấy lại index khi chưa thay đổi hình
	int index;
	if (m_currentIndex > 0)
		index = mario->getCurrentFrame() + 1 - m_frameAnimation->at(m_currentIndex - 1);
	else
		index = mario->getCurrentFrame() + 1 - m_frameAnimation->at(m_frameAnimation->size() - 1);


	// update animation for mario
	if (mario->isBig())
	{
		if (mario->getFSM() == FSM_Mario::RUN)
		{
			// cap nhật lại index big mario
			if (index >= MarioSheet::BIG_MARIO_CHANGE_DIR || index < MarioSheet::BIG_MARIO_RUN)
				index = BIG_MARIO_RUN;
			mario->setCurrentFrame(index);
		}
		else if (mario->getFSM() == FSM_Mario::STAND)
			mario->setCurrentFrame(MarioSheet::BIG_MARIO_STAND);
		else if (mario->getFSM() == FSM_Mario::SIT)
			mario->setCurrentFrame(MarioSheet::BIG_MARIO_SIT);
		else if (mario->getFSM() == FSM_Mario::FALL || mario->getFSM() == FSM_Mario::JUMP)
		{
			if (keyboard->getKeyCode() == DIK_DOWN)
				mario->setCurrentFrame(MarioSheet::BIG_MARIO_SIT);
			else
				mario->setCurrentFrame(MarioSheet::BIG_MARIO_JUMP);
		}
	}
	else if (mario->canShoot())
	{
		if (mario->getFSM() == FSM_Mario::RUN)
		{
			// cap nhật lại index fire mario
			if (index >= MarioSheet::BIG_SUPER_CHANGE_DIR || index < MarioSheet::BIG_SUPER_RUN)
				index = MarioSheet::BIG_SUPER_RUN;
			mario->setCurrentFrame(index);
		}
		else if (mario->getFSM() == FSM_Mario::STAND)
			mario->setCurrentFrame(MarioSheet::BIG_SUPER_STAND);
		else if (mario->getFSM() == FSM_Mario::SIT)
			mario->setCurrentFrame(MarioSheet::BIG_SUPER_SIT);
		else if (mario->getFSM() == FSM_Mario::FALL || mario->getFSM() == FSM_Mario::JUMP)
		{
			if (keyboard->getKeyCode() == DIK_DOWN)
				mario->setCurrentFrame(MarioSheet::BIG_SUPER_SIT);
			else
				mario->setCurrentFrame(MarioSheet::BIG_SUPER_JUMP);
		}
	}
	else
	{
		if (mario->getFSM() == FSM_Mario::RUN)
		{
			// cap nhật lại index small mario
			if (index >= MarioSheet::MARIO_CHANGE_DIR || index < MarioSheet::MARIO_RUN)
				index = MarioSheet::MARIO_RUN;
			mario->setCurrentFrame(index);
		}
		else if (mario->getFSM() == FSM_Mario::STAND)
			mario->setCurrentFrame(MarioSheet::MARIO_STAND);
		else if (mario->getFSM() == FSM_Mario::JUMP || mario->getFSM() == FSM_Mario::FALL)
			mario->setCurrentFrame(MarioSheet::MARIO_JUMP);
	}

	if (m_timeCount == 0)
	{
		if (mario->isBig())
			mario->getStatusStateMachine()->changeState(Big::getInstance());
		else if (mario->canShoot())
			mario->getStatusStateMachine()->changeState(Fire::getInstance());
		else
			mario->getStatusStateMachine()->changeState(mario->getStatusStateMachine()->GetPreviousState());

		// make not effect
		mario->m_effectBig = false;
		mario->m_effectFire = false;
		mario->m_effectSmall = false;
	}
	else
	{
		mario->setCurrentFrame(m_frameAnimation->at(m_currentIndex) + mario->getCurrentFrame());
		m_timeCount--;
		m_currentIndex = ++m_currentIndex >= m_frameAnimation->size() ? 0 : m_currentIndex;
	}	
}

void Star::exit(Mario* mario)
{
	m_frameAnimation = nullptr;
	mario->setStar(false);
}

Star::~Star()
{
	m_frameAnimationBig->clear();
	m_frameAnimationSmall->clear();

	delete m_frameAnimationBig;
	delete m_frameAnimationSmall;
}
#pragma endregion
