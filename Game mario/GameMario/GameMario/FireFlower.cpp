#include "FireFlower.h"
#include "FireFlowerOwnedState.h"
#include "Mario.h"
#include "MarioOwnedState.h"
FireFlower::FireFlower()
{
	// <Xác định frame animation của goomba>
	// Di chuyển
	for (int i = 0; i < 2; i++)
		m_frameList.push_back(Frame(192 + 16 * i, 9, 16, 23));
	// </>

	m_stateMachine = new StateMachine<Enemy>(this);
	m_stateMachine->changeState(FireFlowerMove::getInstance());

	m_typeObject = TypeObject::Moving_Enemy;
	m_flip = SpriteEffect::None;

	m_timeMove = 80;
}


FireFlower::~FireFlower()
{
	delete m_stateMachine;
}


bool FireFlower::isCollision(GameObject* gameObject)
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
			}
			else if (!mario->m_effectSmall)
			{ 
				mario->setVelocity(Collision::getInstance()->getVelocity());
				mario->setDead(true);
			}
		}
	}
	return false;
}