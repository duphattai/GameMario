#include "Enemy.h"
#include "ReSource.h"
#include "KoopaTroopaOwnedState.h"
#include "GoombaOwnedState.h"
#include "KeyBoard.h"
#include "Mario.h"
#include "MarioOwnedState.h"
#include "Global.h"
#include "SoundClass.h"
extern CKeyBoard *keyboard;
extern LPD3DXFONT m_font;

vector<Frame> getFrameAnimationBy(EnemyType newEnemyType)
{
	vector<Frame> frameItem;
	RECT rect;
	switch (newEnemyType)//xác định frame cho animation
	{
	case EnemyType::TortoiseEnemy://Rùa
		for (int i = 0; i < 2; i++)
		{
			// frame i
			rect.left = 96 + 16 * i;
			rect.right = rect.left + 16;
			rect.top = 8;
			rect.bottom = rect.top + 24;

			frameItem.push_back(Frame(0, rect));
		}
		// frame mai rùa ko chân lúc vừa bị đạp
		rect.left = 161;
		rect.right = rect.left + 14;
		rect.top = 17;
		rect.bottom = rect.top + 12;

		frameItem.push_back(Frame(0, rect));

		// frame mai rùa có chân
		rect.left = 177;
		rect.right = rect.left + 14;
		rect.top = 17;
		rect.bottom = rect.top + 15;

		frameItem.push_back(Frame(0, rect));
		break;

	case EnemyType::MushroomEnemy://nấm
		for (int i = 0; i < 2; i++)
		{
			// frame i
			rect.left = 0 + 16 * i;
			rect.right = rect.left + 16;
			rect.top = 16;
			rect.bottom = rect.top + 16;

			frameItem.push_back(Frame(0, rect));
		}
		// frame nấm lúc bị đạp
		rect.left = 32;
		rect.right = rect.left + 16;
		rect.top = 24;
		rect.bottom = rect.top + 8;

		frameItem.push_back(Frame(0, rect));

		break;

	}


	return frameItem;
}

Enemy::Enemy(EnemyType newEnemyType)
{
	m_beAttack = BeAttack::None;
	m_sprite = ReSource::getInstance()->getSprite(IDImage::IMG_ENEMYSHEET);

	m_stateMachine = new StateMachine<Enemy>(this);
	m_frameList = getFrameAnimationBy(newEnemyType);
	m_typeObject = TypeObject::Moving_Enemy;
	m_flip = SpriteEffect::None;
	if (newEnemyType == MushroomEnemy)
		m_stateMachine->changeState(GoombaMove::getInstance());
	else if (newEnemyType == TortoiseEnemy)
		m_stateMachine->changeState(KoopaTroopaMove::getInstance());
}
Enemy::~Enemy()
{
	delete m_stateMachine;
}



bool Enemy::isCollision(GameObject* gameObject)
{
	// xét va chạm với stand position và item
	// nếu item không active thì không xét 
	if (!m_isActive || m_status == StatusObject::DEAD)
		return false;


	Mario* mario = dynamic_cast<Mario*>(gameObject);
	if (mario != nullptr)
	{
		DIR dir = Collision::getInstance()->isCollision(mario, this);
		if (dir != DIR::NONE)
		{
			if (mario->getStatusStateMachine()->isInState(*Star::getInstance()))
			{
				m_beAttack = BeAttack::DeathByGun;
			}
			else if (dir == DIR::TOP && mario->getFSM() == FSM_Mario::FALL)
			{
				m_beAttack = BeAttack::DeathByJump;
				mario->getStateMachine()->changeState(Jumping::getInstance());
			}
			else
				mario->setStatusObject(StatusObject::DEAD);
		}
	}
	else
	{
		int type = gameObject->getTypeObject();
		if (type == TypeObject::Dynamic_Item || type == TypeObject::Dynamic_StandPosition || type == TypeObject::Moving_Enemy)
		{
			DIR dir = Collision::getInstance()->isCollision(this, gameObject);
			if (dir != DIR::NONE)
			{
				m_velocity = Collision::getInstance()->getVelocity();
				if (dir == DIR::LEFT)
				{
					m_flip = SpriteEffect::None;
					if (type == TypeObject::Moving_Enemy)
						gameObject->setFliping(SpriteEffect::Flip);
				}
				else if (dir == DIR::RIGHT)
				{
					m_flip = SpriteEffect::Flip;
					if (type == TypeObject::Moving_Enemy)
						gameObject->setFliping(SpriteEffect::None);
				}
			}
		}
	}

	return false;
}

void Enemy::updateVelocity()
{
	m_stateMachine->update();
}

void Enemy::update()
{
	m_position.x += m_velocity.x;
	m_position.y += m_velocity.y;

	if (keyboard->isKeyDown(DIK_D))
	{
		m_beAttack = BeAttack::DeathByJump;
	}
	else if (keyboard->isPressed(DIK_S))
	{
		m_beAttack = BeAttack::DeathByGun;
	}
}

void Enemy::draw(LPD3DXSPRITE SpriteHandler)
{
	m_sprite->setRect(m_frameList[m_currentFrame].rect);
	if (m_isActive)
	{
		if (m_flip == SpriteEffect::Flip)
			m_scale = D3DXVECTOR2(-1.0f, 1.0f);
		else
			m_scale = D3DXVECTOR2(1.0f, 1.0f);

		if (m_beAttack == BeAttack::DeathByGun)
			m_scale.y = -1.0f;

		m_sprite->draw(SpriteHandler, D3DXVECTOR2(m_position.x + m_sprite->getWidth() / 2, m_position.y + m_sprite->getHeight() / 2), m_scale, m_worldPosition.x, m_worldPosition.y, m_alpha);

		if (m_text.size() != 0)
		{
			m_positionText.y += 2;
			RECT rct;
			rct.left = m_positionText.x - m_worldPosition.x;
			rct.right = rct.left + 20;
			rct.top = VIEW_PORT_Y - (m_positionText.y + m_sprite->getHeight());
			rct.bottom = rct.top + 12;

			m_font->DrawText(NULL, m_text.c_str(), -1, &rct, 0, D3DCOLOR_XRGB(255, 255, 255));
		}
	}
}


void Enemy::setCurrentFrame(int frame)
{
	m_currentFrame = frame;
	m_width = abs(m_frameList[m_currentFrame].rect.right - m_frameList[m_currentFrame].rect.left);
	m_height = abs(m_frameList[m_currentFrame].rect.bottom - m_frameList[m_currentFrame].rect.top);
}