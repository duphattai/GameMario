#include "Goomba.h"
#include "ReSource.h"
#include "GoombaOwnedState.h"
#include "Mario.h"
#include "MarioOwnedState.h"
#include "KoopaTroopa.h"
#include "KoopaTroopaOwnedState.h"
#include "LuckyBox.h"
#include "ItemOwnedState.h"

Goomba::Goomba()
{
	// <Xác định frame animation của goomba>
	// Di chuyển
	for (int i = 0; i < 2; i++)
		m_frameList.push_back(Frame(16 * i, 16, 16, 16));
	// Goomba bị đạp
	m_frameList.push_back(Frame(32, 24, 16, 8));
	// </>

	m_stateMachine = new StateMachine<Enemy>(this);
	m_stateMachine->changeState(GoombaMove::getInstance());

	m_typeObject = TypeObject::Moving_Enemy;
	m_flip = SpriteEffect::None;
}


Goomba::~Goomba()
{
	delete m_stateMachine;
}

bool Goomba::isCollision(GameObject* gameObject)
{
	// xét va chạm với stand position và item
	// nếu item không active thì không xét 
	if (!m_isActive || m_status == StatusObject::DEAD
		|| gameObject->getStatusOBject() == StatusObject::DEAD)
		return false;


	Mario* mario = dynamic_cast<Mario*>(gameObject);
	if (mario != nullptr)
	{
		if (mario->m_unDying) return false;

		DIR dir = Collision::getInstance()->isCollision(mario, this);
		if (dir != DIR::NONE)
		{
			if (mario->getStatusStateMachine()->isInState(*Star::getInstance()))
			{
				m_beAttack = BeAttack::DeathByGun;
				return true;
			}

			mario->setVelocity(Collision::getInstance()->getVelocity());
			if (dir == DIR::TOP && mario->getFSM() == FSM_Mario::FALL)
			{
				m_beAttack = BeAttack::DeathByJump;
				mario->getStateMachine()->changeState(Jumping::getInstance());
			}
			else
			{
				if (!mario->m_effectSmall)
					mario->setDead(true);
			}	
		}
	}
	else// khi đối tượng ko phải là mario
	{
		int type = gameObject->getTypeObject();
		if (type == TypeObject::Dynamic_Item || type == TypeObject::Dynamic_StandPosition || type == TypeObject::Moving_Enemy)
		{
			DIR dir = Collision::getInstance()->isCollision(this, gameObject);
			if (dir != DIR::NONE)
			{
				m_velocity = Collision::getInstance()->getVelocity();
				KoopaTroopa* koopaTroopa = dynamic_cast<KoopaTroopa*>(gameObject);
				if (koopaTroopa != nullptr)//
				{
					if (koopaTroopa->getStateMachine()->isInState(*KoopaTroopaShellMove::getInstance()))
						m_beAttack = BeAttack::DeathByGun;
				}
				else if(dir == DIR::LEFT)
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