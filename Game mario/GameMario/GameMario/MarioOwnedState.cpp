#include "Mario.h"
#include "MarioOwnedState.h"
#include "KeyBoard.h"
#include "SoundClass.h"
#include "Camera.h"
extern CKeyBoard *keyboard; // ý nghĩa là keyboard đã được đinh nghĩa ở đâu đó


// action of mario
#pragma region

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

	keyboard->getState();
	if (keyboard->isPressed(DIK_UP))
		mario->getStateMachine()->changeState(Jumping::getInstance());
	else if (keyboard->isKeyDown(DIK_LEFT))
	{
		mario->setFliping(SpriteEffect::Flip);
		mario->getStateMachine()->changeState(Running::getInstance());
	}	
	else if (keyboard->isKeyDown(DIK_RIGHT))
	{
		mario->setFliping(SpriteEffect::None);
		mario->getStateMachine()->changeState(Running::getInstance());
	}
	else if (keyboard->isKeyDown(DIK_DOWN))
		mario->getStateMachine()->changeState(Sitting::getInstance());
}

void Standing::exit(Mario* mario)
{

}
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

	keyboard->getState();
	// update velocity
	if (keyboard->isKeyDown(DIK_DOWN))
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
	
	if (keyboard->isPressed(DIK_UP))
		mario->getStateMachine()->changeState(Jumping::getInstance());
	else if (mario->getLocation() == Location::LOC_IN_AIR)
		mario->getStateMachine()->changeState(Falling::getInstance());
}

void Running::exit(Mario* mario)
{
}
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

	// cập nhật va chạm
	DIR dir = mario->getDirCollision();
	if (dir != DIR::NONE) // xảy ra va chạm
	{
		if (dir == DIR::BOTTOM) // va chạm bottom thì rớt xuống
			mario->getStateMachine()->changeState(Falling::getInstance());
		else 
			if (mario->getLocation() == Location::LOC_ON_GROUND)
				mario->getStateMachine()->changeState(Standing::getInstance());

		return;
	}


	velocity.y += GRAVITATION;
	// update velocity
	keyboard->getState();
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
	if (velocity.y <= 0) // nếu vận tốc theo y bằng 0 thì chuyển state
		mario->getStateMachine()->changeState(Falling::getInstance());
}

void Jumping::exit(Mario* mario)
{
	//Tài: 9/11 play sound
	if (mario->isBig())
		SoundClass::getInstance()->playWaveFile(IDSounds::Sound_Jump_Super);
	else
		SoundClass::getInstance()->playWaveFile(IDSounds::Sound_Jump_Small);
}
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
	mario->setVelocity(Vector2(mario->getVelocity().x, 0));
}

void Falling::execute(Mario* mario)
{
	Vector2 velocity = mario->getVelocity();
	velocity.y += GRAVITATION * 2;

	keyboard->getState();
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
		if (mario->getVelocity().x != 0)
			mario->getStateMachine()->changeState(Running::getInstance());
		else
			mario->getStateMachine()->changeState(Standing::getInstance());
	}		
}

void Falling::exit(Mario* mario)
{
}
/////////////////////////////////////////////////////////////////////////////////


// Auto animation
AutoAnimation* AutoAnimation::m_instance = 0;

AutoAnimation* AutoAnimation::getInstance()
{
	if (!m_instance)
		m_instance = new AutoAnimation();

	return m_instance;
}

void AutoAnimation::enter(Mario* mario)
{
}

void AutoAnimation::execute(Mario* mario)
{
	
}

void AutoAnimation::exit(Mario* mario)
{
}
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
	m_countTime = 0;

	// dành cho fire mario
	int bigIndex = mario->getCurrentFrame();
	bigIndex = bigIndex > MarioSheet::BIG_MARIO_SIT ? bigIndex - MarioSheet::BIG_SUPER_STAND : bigIndex; // nếu index > index cua big mario thi chuyen index ve big mario

	// make effect mario big -> small
	m_frameAnimation.push_back(bigIndex); // big mario
	m_frameAnimation.push_back(bigIndex + MARIO_STAND); // fire mario
	m_frameAnimation.push_back(bigIndex); // invinbility color 1
	m_frameAnimation.push_back(bigIndex + BIG_SUPER_FIRE_RUN); // invinbility color 2
	m_frameAnimation.push_back(bigIndex); // invinbility color 1
	m_frameAnimation.push_back(bigIndex + MARIO_STAND); // fire mario

	m_timeChangeSprite = 1;
	mario->setTimeAnimation(2);

	// thiết lập âm thanh, chỉ play âm thanh khi được chuyển từ Big, Fire state
	if (mario->getStateMachine()->GetPreviousState() == Big::getInstance() || mario->getStateMachine()->GetPreviousState() == Fire::getInstance())
		SoundClass::getInstance()->playWaveFile(IDSounds::Sound_Pipe);
}

void Small::execute(Mario* mario)
{
	if (mario->m_effectSmall)
	{
		if (m_currentIndex == m_frameAnimation.size())
		{	
			mario->m_effectSmall = false;
			m_countTime = 70;
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
		// update animation of mario
		if (mario->getFSM() == FSM_Mario::RUN)
		{
			int index = mario->getCurrentFrame() + 1;
			mario->setTimeAnimation(mario->getTimeAnimation() - 1);
			if (mario->getTimeAnimation() == 0)
			{
				mario->setTimeAnimation(2);
				// cap nhật lại index small mario
				if (index >= MarioSheet::MARIO_CHANGE_DIR || index < MarioSheet::MARIO_RUN)
					index = MarioSheet::MARIO_RUN;

				mario->setCurrentFrame(index);
			}
		}
		else if (mario->getFSM() == FSM_Mario::STAND || mario->getFSM() == FSM_Mario::SIT)
			mario->setCurrentFrame(MarioSheet::MARIO_STAND);
		else if (mario->getFSM() == FSM_Mario::JUMP || mario->getFSM() == FSM_Mario::FALL)
			mario->setCurrentFrame(MarioSheet::MARIO_JUMP);

		// update mờ ảnh sau khi thực hiện smaller
		if (--m_countTime > 0)
		{
			D3DXCOLOR colorAlpha = mario->getAlphaColor();
			colorAlpha *= 255;
			static int temp = -25;
			if (colorAlpha.a < 155 || colorAlpha.a > 255)
				temp *= -1;
			colorAlpha.a += temp;
			colorAlpha /= 255;
			mario->setAlphaColor(colorAlpha);
		}
		else
			mario->setAlphaColor(D3DCOLOR_ARGB(255, 255, 255, 255));


		if (mario->isBig())
		{
			mario->getStatusStateMachine()->changeState(Big::getInstance());
		}
		else if (mario->isDead()) // không gọi lại
		{
			SoundClass::getInstance()->playWaveFile(IDSounds::Sound_MarioDie);
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
	m_timeChangeSprite = 1;

	//Tài: 9/11 play sound
	SoundClass::getInstance()->playWaveFile(IDSounds::Sound_PowerUp);
}

void Big::execute(Mario* mario)
{
	if (mario->m_effectBig) // hiệu ứng small -> bigger
	{
		if (m_currentIndex == m_frameAnimation.size()) // kết thúc hiêu ứng
			mario->m_effectBig = false;
		else
			mario->setCurrentFrame(m_frameAnimation[m_currentIndex++]);
	}
	else
	{
		// update animation of mario theo vận tốc
		if (mario->getFSM() == FSM_Mario::RUN)
		{
			int index = mario->getCurrentFrame();
			if (abs(mario->getVelocity().x) <= 3.0f) // khi v <= 3.0 => thời gian chuyển đỗi mỗi frame là 1s
			{
				if (m_timeChangeSprite-- == 0)
				{
					index++;
					m_timeChangeSprite = 1; // set time change
				}
			}
			else index++; // => thời gian chuyển đỗi mỗi frame là 0s
			

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
			SoundClass::getInstance()->playWaveFile(IDSounds::Sound_Vine);
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

	SoundClass::getInstance()->playWaveFile(IDSounds::Sound_MarioDie);
}

void Dead::execute(Mario* mario)
{
	Vector2 velocity = mario->getVelocity();
	velocity.x = 0;
	velocity.y--;
	mario->setVelocity(velocity);
}

void Dead::exit(Mario* mario)
{
	mario->setFSM(FSM_Mario::FALL);
	mario->setIsBig(false);
	mario->setCanShoot(false);
	mario->setCurrentFrame(MarioSheet::MARIO_JUMP);
	mario->m_effectSmall = false;
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
	m_frameAnimation.push_back(bigIndex + BIG_SUPER_STAND); // fire mario
	m_frameAnimation.push_back(bigIndex + BIG_INVINCIBILITY_COLOR_1_STAND); // invinbility color 1
	m_frameAnimation.push_back(bigIndex + BIG_INVINCIBILITY_COLOR_2_STAND); // invinbility color 2
	m_frameAnimation.push_back(bigIndex + BIG_INVINCIBILITY_COLOR_1_STAND); // invinbility color 1
	m_frameAnimation.push_back(bigIndex + BIG_SUPER_STAND); // fire mario
	m_frameAnimation.push_back(bigIndex + BIG_INVINCIBILITY_COLOR_1_STAND); // invinbility color 1
	m_frameAnimation.push_back(bigIndex + BIG_SUPER_STAND); // fire mario

	m_timeChangeSprite = 2;

	SoundClass::getInstance()->playWaveFile(IDSounds::Sound_PowerUp);
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
			// cập nhật animation theo vận tốc
			int index = mario->getCurrentFrame();
			if (abs(mario->getVelocity().x) <= 3.0f) // khi v <= 3.0 => thời gian chuyển đỗi mỗi frame là 1s
			{
				if (m_timeChangeSprite-- == 0)
				{
					index++;
					m_timeChangeSprite = 1; // set time change
				}
			}
			else index++;
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

		// bắn súng
		if (keyboard->isPressed(DIK_SPACE) && mario->getFSM() != FSM_Mario::SIT)
		{
			// thiết lập frame bắn súng
			if (mario->getCurrentFrame() == MarioSheet::BIG_SUPER_STAND)
				mario->setCurrentFrame(mario->getCurrentFrame() + 7);
			else
				mario->setCurrentFrame(mario->getCurrentFrame() + 6);

			// xét tọa độ đạn hợp lý
			Vector2 position = mario->getPosition();
			if (mario->getFliping() == SpriteEffect::None)
				position.x += mario->getWidth();
			Vector2 camera = Camera::getInstance()->getViewport();

			// bắn súng
			mario->getGun()->shoot(position.x, position.y, camera.x, camera.y, mario->getFliping());
		}

		if (mario->isDead())
			mario->getStatusStateMachine()->changeState(Small::getInstance());
		else if (mario->isStar())
			mario->getStatusStateMachine()->changeState(Star::getInstance());
	}
}

void Fire::exit(Mario* mario)
{
	mario->setDead(false);
	m_frameAnimation.clear();
}

///////////  efect start
Star* Star::m_instance = 0;

Star::Star()
{
	m_frameAnimationBig = new vector<int>();
	m_frameAnimationBig->push_back(MarioSheet::BIG_MARIO_STAND); // big mario
	m_frameAnimationBig->push_back(MarioSheet::BIG_SUPER_STAND); // fire mario
	m_frameAnimationBig->push_back(MarioSheet::BIG_INVINCIBILITY_COLOR_1_STAND); // invinbility color 1
	m_frameAnimationBig->push_back(MarioSheet::BIG_INVINCIBILITY_COLOR_2_STAND); // fire mario
	m_frameAnimationBig->push_back(MarioSheet::BIG_INVINCIBILITY_COLOR_1_STAND); // invinbility color 1
	m_frameAnimationBig->push_back(MarioSheet::BIG_SUPER_STAND); // fire mario
	m_frameAnimationBig->push_back(MarioSheet::BIG_MARIO_STAND); // big mario

	m_frameAnimationSmall = new vector<int>();
	m_frameAnimationSmall->push_back(MarioSheet::BIG_MARIO_STAND); // mario
	m_frameAnimationSmall->push_back(SMALL_SUPER_FIRE_STAND - MARIO_STAND); // fire mario
	m_frameAnimationSmall->push_back(SMALL_INVINCIBILITY_COLOR_1_STAND - MARIO_STAND); // invinbility color 1
	m_frameAnimationSmall->push_back(SMALL_INVINCIBILITY_COLOR_2_STAND - MARIO_STAND); // fire mario
	m_frameAnimationSmall->push_back(SMALL_INVINCIBILITY_COLOR_1_STAND - MARIO_STAND); // invinbility color 1
	m_frameAnimationSmall->push_back(SMALL_SUPER_FIRE_STAND - MARIO_STAND); // fire mario
	m_frameAnimationSmall->push_back(BIG_MARIO_STAND); // mario

	m_frameAnimationFire = new vector<int>();
	m_frameAnimationFire->push_back(MarioSheet::BIG_MARIO_STAND - MarioSheet::BIG_SUPER_STAND); // big mario
	m_frameAnimationFire->push_back(MarioSheet::BIG_SUPER_STAND - MarioSheet::BIG_SUPER_STAND); // fire mario
	m_frameAnimationFire->push_back(MarioSheet::BIG_INVINCIBILITY_COLOR_1_STAND - MarioSheet::BIG_SUPER_STAND); // invinbility color 1
	m_frameAnimationFire->push_back(MarioSheet::BIG_INVINCIBILITY_COLOR_2_STAND - MarioSheet::BIG_SUPER_STAND); // fire mario
	m_frameAnimationFire->push_back(MarioSheet::BIG_INVINCIBILITY_COLOR_1_STAND - MarioSheet::BIG_SUPER_STAND); // invinbility color 1
	m_frameAnimationFire->push_back(MarioSheet::BIG_SUPER_STAND - MarioSheet::BIG_SUPER_STAND); // fire mario
	m_frameAnimationFire->push_back(MarioSheet::BIG_MARIO_STAND - MarioSheet::BIG_SUPER_STAND); // big mario
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
	m_timeCount = 200;
	m_timeChangeSprite = 2;
}

void Star::execute(Mario* mario)
{
	// update status of mario
	mario->setIsBig(mario->isBig());
	mario->setCanShoot(mario->canShoot());
	if (mario->isBig())
		m_frameAnimation = m_frameAnimationBig;
	else if (mario->canShoot())
		m_frameAnimation = m_frameAnimationFire;
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

		// bắn súng
		if (keyboard->isPressed(DIK_SPACE) && mario->getFSM() != FSM_Mario::SIT)
		{
			// xét tọa độ đạn hợp lý
			Vector2 position = mario->getPosition();
			if (mario->getFliping() == SpriteEffect::None)
				position.x += mario->getWidth();
			Vector2 camera = Camera::getInstance()->getViewport();

			// bắn súng
			mario->getGun()->shoot(position.x, position.y, camera.x, camera.y, mario->getFliping());
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


	if (m_timeCount-- <= 0) // hết thời gian hiệu ứng star
	{
		// chuyển về state trước đó
		if (mario->isBig())
			mario->getStatusStateMachine()->changeState(Big::getInstance());
		else if (mario->canShoot())
			mario->getStatusStateMachine()->changeState(Fire::getInstance());
		else
			mario->getStatusStateMachine()->changeState(Small::getInstance());

		// make not effect
		mario->m_effectBig = false;
		mario->m_effectFire = false;
		mario->m_effectSmall = false;
	}
	else
	{
		mario->setCurrentFrame(m_frameAnimation->at(m_currentIndex) + mario->getCurrentFrame());
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
	m_frameAnimationFire->clear();

	delete m_frameAnimationBig;
	delete m_frameAnimationSmall;
	delete m_frameAnimationFire;
}
#pragma endregion
