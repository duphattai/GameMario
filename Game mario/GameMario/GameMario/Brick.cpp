#include "Brick.h"
#include "ReSource.h"
#include "ItemOwnedState.h"
#include "Mario.h"
#include "Enemy.h"
Brick::Brick(Vector2 position, IDMap idMap)
{
	m_position.x = position.x;
	m_position.y = position.y;
	m_width = 16;
	m_height = 16;


	// hard code
	// tọa độ mini brick trong items.png
	for (int i = 0; i < 2; i++)
		m_frameList.push_back(Frame(68 + ((int)idMap % 2) * 144, 20 + 16 * i, 8, 8));


	m_framePosition.push_back(Vector2(position.x, position.y));
	m_framePosition.push_back(Vector2(position.x + 8, position.y));
	m_framePosition.push_back(Vector2(position.x, position.y + 8));
	m_framePosition.push_back(Vector2(position.x + 8, position.y + 8));


	m_isBreak = false;
	m_velocity = Vector2(0, 0);
	m_timeAnimation = 3;
	m_currentFrame = 0;
	m_stateMachine = new StateMachine<Brick>(this);
	m_stateMachine->changeState(BrickIdle::getInstance());

	m_index = 2 + 66 * ((int)idMap % 2);
}
Brick::~Brick()
{
	delete m_stateMachine;
}

void Brick::update()
{
	m_position.x += m_velocity.x;
	m_position.y += m_velocity.y;
}
void Brick::updateVelocity()
{
	if (m_isActive)
		m_stateMachine->update();
}

bool Brick::isCollision(GameObject* gameObject)
{
	if (m_isBreak) return false;

	Enemy* enemy = dynamic_cast<Enemy*>(gameObject);
	Mario* mario = dynamic_cast<Mario*>(gameObject);
	if (mario != nullptr)
	{
		if (mario->isDead()) return false;

		DIR dir = Collision::getInstance()->isCollision(mario, this);
		if (dir != DIR::NONE)
		{
			if (mario->getDirCollision() == DIR::NONE)
				mario->setDirCollision(dir);

			mario->setVelocity(Collision::getInstance()->getVelocity());
			if (dir == DIR::BOTTOM)
			{
				if (!mario->isBig() && !mario->canShoot())
					m_makeEffect = true;
				else
				{
					m_status = StatusObject::DEAD;
					m_isBreak = true;
				}
			}
			else if (dir == DIR::TOP)
			{
				if (mario->getFSM() == FSM_Mario::FALL || mario->getFSM() == FSM_Mario::RUN) // fall gặp vật cản
					mario->setLocation(Location::LOC_ON_GROUND);
			}
		}	
	}
	else if (enemy != nullptr)
	{
		if (enemy->getStatusOBject() == StatusObject::DEAD) return false;

		DIR dir = Collision::getInstance()->isCollision(enemy, this);
		if (dir == DIR::TOP)
		{
			enemy->setVelocity(Collision::getInstance()->getVelocity());
			if (m_stateMachine->isInState(*BrickEffect::getInstance())
				|| m_stateMachine->isInState(*BrickBroken::getInstance()))
				enemy->setAttack(BeAttack::DeathByGun);
		}
	}

	return false;
}


void Brick::draw(LPD3DXSPRITE SpriteHandler)
{
	if (!m_isActive) return;

	if (!m_isBreak)
	{
		m_sprite = ReSource::getInstance()->getSprite(IDImage::IMG_TILEMAP);
		m_sprite->setIndex(m_index);
		GameObject::draw(SpriteHandler);
	}
	else
	{
		m_sprite = ReSource::getInstance()->getSprite(IDImage::IMG_ITEMSHEET);
		for each (Vector2 item in m_framePosition)
		{
			m_sprite->setRect(m_frameList[m_currentFrame].rect);
			m_position.x = item.x;
			m_position.y = item.y;

			GameObject::draw(SpriteHandler);
		}
	}
}