#include "KoopaTroopa.h"
#include "KoopaTroopaOwnedState.h"
#include "Mario.h"
#include "MarioOwnedState.h"

KoopaTroopa::KoopaTroopa()
{
	m_stateMachine = new StateMachine<Enemy>(this);
	m_stateMachine->changeState(KoopaTroopaMove::getInstance());

	m_typeObject = TypeObject::Moving_Enemy;
	m_flip = SpriteEffect::None;
}


KoopaTroopa::~KoopaTroopa()
{
	delete m_stateMachine;
}


bool KoopaTroopa::isCollision(GameObject* gameObject)
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
			// Nếu mario ở trang thái star
			if (mario->getStatusStateMachine()->isInState(*Star::getInstance()))
			{
				m_beAttack = BeAttack::DeathByGun;
				return true;
			}

			mario->setVelocity(Collision::getInstance()->getVelocity());			
			// Nếu enemy ở trạng thái shell not move
			if (m_stateMachine->isInState(*KoopaTroopaShellNotMove::getInstance()))
			{
				m_beAttack = BeAttack::DeathByJump;
				if (dir == DIR::RIGHT)
					m_flip = SpriteEffect::None;
				else
					m_flip = SpriteEffect::Flip;
			}
			else if (dir == DIR::TOP && mario->getFSM() == FSM_Mario::FALL)
			{
				m_beAttack = BeAttack::DeathByJump;
				mario->getStateMachine()->changeState(Jumping::getInstance());
			}
			else if (!mario->m_effectSmall)
					mario->setDead(true);
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
				if (m_stateMachine->isInState(*KoopaTroopaShellMove::getInstance()))
				{
					Enemy* enemy = dynamic_cast<Enemy*>(gameObject);
					if (enemy != nullptr)
					{
						enemy->setAttack(BeAttack::DeathByGun);
						return true;
					}
				}
				
				if (dir == DIR::LEFT)
				{
					// xác định hướng đi
					m_flip = SpriteEffect::None; 
					if (type == TypeObject::Moving_Enemy)
						gameObject->setFliping(SpriteEffect::Flip);
				}
				else if (dir == DIR::RIGHT)
				{
					// xác định hướng đi
					m_flip = SpriteEffect::Flip;
					if (type == TypeObject::Moving_Enemy)
						gameObject->setFliping(SpriteEffect::None);
				}
			}
		}
	}

	return false;
}