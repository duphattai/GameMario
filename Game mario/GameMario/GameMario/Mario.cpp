#include "Mario.h"
#include "ReSource.h"
#include "Global.h"
#include <fstream>
#include "MarioOwnedState.h"
#include "LuckyBox.h"
#include "Brick.h"
#include "Coin.h"
#include "Camera.h"
#include "Flag.h"
#include "FloatingBar.h"
using namespace std;

Mario* Mario::m_instance = 0;

Mario::Mario()
{
	ifstream file;
	file.open("Resources//mario.txt");

	char s[1000];
	for (int i = 0; i < 5; i++)
	{
		file.getline(s, 1000, '\n');
	}

	while (!file.eof())
	{
		int Id;
		int index;
		int x;
		int y;
		int width;
		int height;

		file >> Id >> index >> x >> y >> width >> height;

		RECT rect;
		rect.left = x;
		rect.right = x + width;
		rect.top = y;
		rect.bottom = y + height;

		Frame frame(Id, rect);
		frameList.push_back(frame);
	}
	file.close();


	m_sprite = ReSource::getInstance()->getSprite(IDImage::IMG_MARIOSHEET);
	m_MaxVelocity = Vector2(4.0f, 10.0f);
	m_MinVelocity = Vector2(-4.0f, -10.0f);


	//m_worldPosition = Vector2(0, VIEW_PORT_Y);
	m_lives = 3;
	m_isBig = false;
	m_canShoot = false;
	m_isStar = false;

	m_effectBig = false;
	m_effectFire = false;

	m_stateMachine = NULL;
	m_statusStateMachine = NULL;
	
	m_effectSmall = false;

	m_gun = new Gun(2);
}

Mario::~Mario()
{
	delete Falling::getInstance();
	delete Running::getInstance();
	delete Standing::getInstance();
	delete Jumping::getInstance();
	delete Sitting::getInstance();

	delete Star::getInstance();
	delete Small::getInstance();
	delete Dead::getInstance();
	delete Big::getInstance();
	delete Fire::getInstance();

	delete m_stateMachine;
	delete m_statusStateMachine;
}

void Mario::initialize()
{
	m_isBig = false;
	m_canShoot = false;
	m_isStar = false;

	m_effectBig = false;
	m_effectFire = false;

	if (m_stateMachine == NULL)
		m_stateMachine = new StateMachine<Mario>(this);
	m_stateMachine->changeState(Falling::getInstance());

	if (m_statusStateMachine == NULL)
		m_statusStateMachine = new StateMachine<Mario>(this);
	m_statusStateMachine->changeState(Small::getInstance());
	m_effectSmall = false;
}

Mario* Mario::getInstance()
{
	if (m_instance == NULL)
		m_instance = new Mario();

	return m_instance;
}

void Mario::draw(LPD3DXSPRITE SpriteHandler)
{
	setWorldPosition(Camera::getInstance()->getViewport());

	m_sprite->setRect(frameList[m_currentFrame].rect);
	GameObject::draw(SpriteHandler);

	m_gun->draw(SpriteHandler);
}

void Mario::update()
{
	// không hiệu ứng small, fire, big mới update position
	if (!m_effectBig && !m_effectFire && !m_effectSmall)
	{
		m_position.x += m_velocity.x;
		m_position.y += m_velocity.y;
	}
	

	// mario không thể di chuyển khỏi camera
	if (m_position.x < Camera::getInstance()->getViewport().x)
		m_position.x = Camera::getInstance()->getViewport().x;
}

void Mario::updateVelocity()
{
	if (!m_effectBig && !m_effectFire && !m_effectSmall)
		m_stateMachine->update();
	
	m_statusStateMachine->update();
	
	// 24/10 
	// set default, dùng để xét va chạm di chuyển trên map
	setDirCollision(DIR::NONE);
	setLocation(Location::LOC_IN_AIR);
}

Box Mario::getBouding()
{
	GameObject::getBouding();
	
	m_box.x += 2;
	m_box.width -= 4;
	m_box.y += 1;
	m_box.height -= 2;
	
	return m_box;
}

bool Mario::isCollision(GameObject* gameObject)
{
	int type = gameObject->getTypeObject();
	if (type == TypeObject::Dynamic_TiledMap   // không xét va cham với tiled map 
		|| gameObject->getStatusOBject() == StatusObject::DEAD || !gameObject->isActive()
		|| m_stateMachine->isInState(*AutoAnimation::getInstance())) 
		return false;


	// xét va chạm với item in luckybox
	if (type == TypeObject::Dynamic_Item)
	{
		LuckyBox* luckyBox = dynamic_cast<LuckyBox*>(gameObject);
		// chỉ xét va chạm với item in box khi active = true và trạng thái alive
		if (luckyBox != nullptr && luckyBox->getItem()->isActive() && 
			luckyBox->getItem()->getStatusOBject() == StatusObject::ALIVE && 
			luckyBox->getTypeItem() != LuckyBoxsType::IT_COIN)
		{
			if (Collision::getInstance()->isCollision(this, luckyBox->getItem()) != DIR::NONE)
			{
				m_velocity = Collision::getInstance()->getVelocity();
				luckyBox->getItem()->setStatusObject(StatusObject::DEAD);

				// update state for mario
				int typeItem = luckyBox->getTypeItem();
				if (typeItem == LuckyBoxsType::IT_MUSHROOM_BIGGER)
				{
					m_canShoot = false;
					m_isBig = true;
				}	
				else if (typeItem == LuckyBoxsType::IT_STAR)
					m_isStar = true;
				else if (typeItem == LuckyBoxsType::IT_GUN)
				{
					m_canShoot = true;
					m_isBig = false;
				}
				else if (typeItem == LuckyBoxsType::IT_MUSHROOM_UP)
					m_lives++;
			}
		}
	}

	// xét va chạm với enemy, standposition, item
	DIR dir = Collision::getInstance()->isCollision(this, gameObject);
	if (dir != DIR::NONE)
	{
		//m_velocity = m_checkCollision->getVelocity();
		if (type == TypeObject::Dynamic_StandPosition) // trường hợp với stand position
		{
			m_velocity = Collision::getInstance()->getVelocity();
			if ((getFSM() == FSM_Mario::FALL || getFSM() == FSM_Mario::RUN) && dir == DIR::TOP) // fall gặp vật cản
					setLocation(Location::LOC_ON_GROUND);

			if (getDirCollision() == DIR::NONE)
				setDirCollision(dir);
		}
		else if (type == TypeObject::Moving_Enemy) // va chạm với enemy
		{

		}
		else if (type == TypeObject::Dynamic_Item) // va chạm với item
		{
			Coin* coin = dynamic_cast<Coin*>(gameObject);
			LuckyBox* luckyBox = dynamic_cast<LuckyBox*>(gameObject);
			Brick* brick = dynamic_cast<Brick*>(gameObject);
			Flag* flag = dynamic_cast<Flag*>(gameObject);
			FloatingBar *bar = dynamic_cast<FloatingBar*>(gameObject);

			if (bar != nullptr)
			{
				if (getDirCollision() == DIR::NONE)
					setDirCollision(dir);

				m_velocity = Collision::getInstance()->getVelocity();
				if (dir == DIR::TOP)
				{
					//// thiết lập vận tốc
					switch (bar->getTypeFloatingBar())
					{
					case FloatingBarMove::MoveUp:
						m_velocity.y += bar->getVelocity().y;
						break;
					case FloatingBarMove::MoveLeft: case FloatingBarMove::MoveRight:
						m_velocity.x += bar->getVelocity().x;
						break;
					}
					
					if (getFSM() == FSM_Mario::FALL || getFSM() == FSM_Mario::RUN) // fall gặp vật cản
						setLocation(Location::LOC_ON_GROUND);
				}
			}
			else if (flag != nullptr)
			{
				m_velocity = Collision::getInstance()->getVelocity();
				flag->setMakeEffect(true);
				AutoAnimation::getInstance()->m_type = AutoAnimationType::AutoAnimationMoveOnGroundIntoCastle;
				m_stateMachine->changeState(AutoAnimation::getInstance());
			}
			else if (coin != nullptr)
			{
				coin->setStatusObject(StatusObject::DEAD);
				return true;
			}
			else if (luckyBox != nullptr)
			{
				if (getDirCollision() == DIR::NONE)
					setDirCollision(dir);

				m_velocity = Collision::getInstance()->getVelocity();
				if (dir == DIR::BOTTOM)
				{
					luckyBox->setMakeEffect(true);
				}
				else if (dir == DIR::TOP)
				{
					if (getFSM() == FSM_Mario::FALL || getFSM() == FSM_Mario::RUN) // fall gặp vật cản
						setLocation(Location::LOC_ON_GROUND);
				}	
			}
			else if (brick != nullptr)
			{
				if (getDirCollision() == DIR::NONE)
					setDirCollision(dir);

				m_velocity = Collision::getInstance()->getVelocity();
				if (dir == DIR::BOTTOM)
				{
					if (!m_isBig && !m_canShoot)
						brick->setMakeEffect(true);
					else
					{
						brick->setStatusObject(StatusObject::DEAD);
						brick->setIsBreak(true);
					}
				}
				else if (dir == DIR::TOP)
				{
					if (getFSM() == FSM_Mario::FALL || getFSM() == FSM_Mario::RUN) // fall gặp vật cản
						setLocation(Location::LOC_ON_GROUND);
				}
			}
		}

		return true;
	}

	return false;
}

void Mario::setVelocity(Vector2 velocity)
{
	m_velocity = velocity;
	Vector2 maxVelocity = m_MaxVelocity;
	Vector2 minVelocity = m_MinVelocity;

	// thiết lập max, min cho trường hợp small
	if (!m_isBig && !m_canShoot)
	{
		maxVelocity.x--;
		minVelocity.y++;
	}

	if (m_velocity.x >= maxVelocity.x)
		m_velocity.x = maxVelocity.x;
	else if (m_velocity.x <= minVelocity.x)
		m_velocity.x = minVelocity.x;

	if (m_velocity.y >= maxVelocity.y)
		m_velocity.y = maxVelocity.y;
	else if (m_velocity.y <= minVelocity.y)
		m_velocity.y = minVelocity.y;
	
}

void Mario::setCurrentFrame(int frame)
{
	m_currentFrame = frame; 
	m_width = abs(frameList[m_currentFrame].rect.left - frameList[m_currentFrame].rect.right);
	m_height = abs(frameList[m_currentFrame].rect.top - frameList[m_currentFrame].rect.bottom);
}
