#include "Bowser.h"
#include "BowserOwnedState.h"
#include "Mario.h"

Bowser::Bowser()
{
	m_stateMachine = new StateMachine<Enemy>(this);
	m_stateMachine->changeState(BowserMove::getInstance());

	m_typeObject = TypeObject::Moving_Enemy;
	m_flip = SpriteEffect::None;

	m_manageHammer = new ManageHammer(1);
	m_manageBulletFire = new ManageBulletFire(1);
}


Bowser::~Bowser()
{
	delete m_manageHammer;
	delete m_manageBulletFire;
	delete m_stateMachine;

	// Hủy vùng nhớ
	BowserMove::getInstance()->release();
	BowserAttackByFire::getInstance()->release();
	BowserAttackByHammer::getInstance()->release();
	BowserJump::getInstance()->release();
	BowserDieByGun::getInstance()->release();
}


bool Bowser::isCollision(GameObject* gameObject)
{
	// xét va chạm với stand position và item
	// nếu item không active thì không xét 
	if (!m_isActive || m_status == StatusObject::DEAD
		|| gameObject->getStatusOBject() == StatusObject::DEAD)
		return false;

	// Xét va cham cho hammer
	m_manageHammer->isCollision(gameObject);
	m_manageBulletFire->isCollision(gameObject);

	Mario* mario = dynamic_cast<Mario*>(gameObject);
	if (mario != nullptr)
	{
		if (mario->m_unDying) return false;

		DIR dir = Collision::getInstance()->isCollision(mario, this);
		if (dir != DIR::NONE)
		{
			mario->setVelocity(Collision::getInstance()->getVelocity());
			if (!mario->m_effectSmall)
				mario->setDead(true);
		}
	}
	else
	{
		int type = gameObject->getTypeObject();
		if (type == TypeObject::Dynamic_Item || type == TypeObject::Dynamic_StandPosition)
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
				else if (dir == DIR::TOP)
					m_location = Location::LOC_ON_GROUND;
			}
		}
	}

	return false;
}

void Bowser::draw(LPD3DXSPRITE SpriteHandler)
{
	m_manageBulletFire->draw(SpriteHandler);
	Enemy::draw(SpriteHandler);
	m_manageHammer->draw(SpriteHandler);
}

void Bowser::update()
{
	Enemy::update();
	m_manageHammer->update();
	m_manageBulletFire->update();
}

void Bowser::updateVelocity()
{
	Enemy::updateVelocity();
	m_manageHammer->updateVelocity();
	m_manageBulletFire->updateVelocity();
}