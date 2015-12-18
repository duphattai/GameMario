#include "KoopaTroopaFly.h"
#include "KoopaTroopaFlyOwnedState.h"
#include "Mario.h"
#include "MarioOwnedState.h"
#include "KoopaTroopaOwnedState.h"

KoopaTroopaFly::KoopaTroopaFly()
{
	// <Xác định frame animation của koopatroopafly>
	for (int i = 0; i < 4; i++)
		m_frameList.push_back(Frame(96 + 16 * i, 8, 16, 24));
	// frame mai rùa ko chân lúc vừa bị đạp
	m_frameList.push_back(Frame(161, 17, 14, 12));
	// frame mai rùa có chân
	m_frameList.push_back(Frame(177, 17, 14, 15));
	// </>

	m_stateMachine = new StateMachine<Enemy>(this);
	m_stateMachine->changeState(KoopaTroopaFlyMoveFly::getInstance());

	m_typeObject = TypeObject::Moving_Enemy;
	m_flip = SpriteEffect::None;
}


KoopaTroopaFly::~KoopaTroopaFly()
{
	delete m_stateMachine;
}

bool KoopaTroopaFly::isCollision(GameObject* gameObject)
{
	// xét va chạm với stand position và item
	// nếu item không active thì không xét 
	if (!m_isActive || m_status == StatusObject::DEAD
		|| gameObject->getStatusOBject() == StatusObject::DEAD)
		return false;


	Mario* mario = dynamic_cast<Mario*>(gameObject);
	if (mario != nullptr)
	{
		DIR dir = Collision::getInstance()->isCollision(mario, this);
		if (dir != DIR::NONE)
		{
			mario->setVelocity(Collision::getInstance()->getVelocity());
			// Nếu mario ở trang thái star
			if (mario->getStatusStateMachine()->isInState(*Star::getInstance()))
			{
				m_beAttack = BeAttack::DeathByGun;
			}
			// Nếu enemy ở trạng thái shell not move
			else if (m_stateMachine->isInState(*KoopaTroopaShellNotMove::getInstance()))
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
					alpha = 0;
					time = 0;
					// xác định hướng đi
					m_flip = SpriteEffect::None;
					if (type == TypeObject::Moving_Enemy)
						gameObject->setFliping(SpriteEffect::Flip);
				}
				else if (dir == DIR::RIGHT)
				{
					alpha = 0;
					time = 0;
					// xác định hướng đi
					m_flip = SpriteEffect::Flip;
					if (type == TypeObject::Moving_Enemy)
						gameObject->setFliping(SpriteEffect::None);
				}
				else if (dir == DIR::TOP)
				{
					alpha = 3.14 / 3;
					time = 0;
				}
			}
		}
	}

	return false;
}
