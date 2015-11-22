#include "Mario.h"
#include "ReSource.h"
#include "Global.h"
#include <fstream>
#include "MarioOwnedState.h"
#include "LuckyBox.h"
#include "Brick.h"
#include "Coin.h"


using namespace std;

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
	m_MaxVelocity = Vector2(3.0f, 10.0f);
	m_MinVelocity = Vector2(-3.0f, -10.0f);


	m_worldPosition = Vector2(0, VIEW_PORT_Y);
	m_lives = 3;
	m_isBig = false;
	m_canShoot = false;
	m_isStar = false;

	m_effectBig = false;
	m_effectFire = false;

	m_stateMachine = new StateMachine<Mario>(this);
	m_stateMachine->changeState(Falling::getInstance());

	m_statusStateMachine = new StateMachine<Mario>(this);
	m_statusStateMachine->changeState(Small::getInstance());
	m_effectSmall = false;

	m_checkCollision = new Collision();

	m_gun = new Gun(2);
}

void Mario::draw(LPD3DXSPRITE SpriteHandler)
{
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
	

	// make mario not move off camera
	if (m_position.x < m_worldPosition.x)
		m_position.x = m_worldPosition.x;

	// update camera just move right
	if (m_worldPosition.x < m_position.x - SCREEN_WIDTH / 2)
		m_worldPosition.x = m_position.x - SCREEN_WIDTH / 2;

	m_gun->update();
}

void Mario::updateVelocity()
{
	if (!m_effectBig && !m_effectFire && !m_effectSmall)
		m_stateMachine->update();
	
	m_statusStateMachine->update();
	
	m_gun->updateVelocity();
	// 24/10 
	// set default, dùng để xét va chạm di chuyển trên map
	setDirCollision(DIR::NONE);
	setLocation(Location::LOC_IN_AIR);
}


Box Mario::getBouding()
{
	GameObject::getBouding();

	//hard code
	m_box.x += 6;
	m_box.width = 4;
	m_box.y += 2;
	m_box.height -= 8;

	return m_box;
}

bool Mario::isCollision(GameObject* gameObject)
{
	int type = gameObject->getTypeObject();
	if (type == TypeObject::Dynamic_TiledMap || gameObject->getStatusOBject() == StatusObject::DEAD || !gameObject->isActive()) // không xét va cham với tiled map 
		return false;


	// xét va chạm với item in luckybox
	if (type == TypeObject::Dynamic_Item)
	{
		LuckyBox* luckyBox = dynamic_cast<LuckyBox*>(gameObject);
		// chỉ xét va chạm với item in box khi active = true và trạng thái alive
		if (luckyBox != nullptr && luckyBox->getItem()->isActive() && 
			luckyBox->getItem()->getStatusOBject() == StatusObject::ALIVE && 
			luckyBox->getType() != LuckyBoxsType::IT_COIN && luckyBox->getType() != LuckyBoxsType::IT_BRICK)
		{
			if (m_checkCollision->isCollision(this, luckyBox->getItem()) != DIR::NONE)
			{
				m_velocity = m_checkCollision->getVelocity();
				luckyBox->getItem()->setStatusObject(StatusObject::DEAD);

				// update state for mario
				int typeItem = luckyBox->getType();
				if (typeItem == LuckyBoxsType::IT_MUSHROOM_BIGGER)
					m_isBig = true;
				else if (typeItem == LuckyBoxsType::IT_STAR)
					m_isStar = true;
				else if (typeItem == LuckyBoxsType::IT_GUN)
					m_canShoot = true;
				else if (typeItem == LuckyBoxsType::IT_MUSHROOM_UP)
					m_lives++;
			}
		}
	}

	// xét va chạm với enemy, standposition, item
	DIR dir = m_checkCollision->isCollision(this, gameObject);
	if (dir != DIR::NONE)
	{
		//m_velocity = m_checkCollision->getVelocity();
		if (type == TypeObject::Dynamic_StandPosition) // trường hợp với stand position
		{
			m_velocity = m_checkCollision->getVelocity();
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
			if (getDirCollision() == DIR::NONE)
				setDirCollision(dir);

			Coin* coin = dynamic_cast<Coin*>(gameObject);
			if (coin != nullptr)
			{
				coin->setStatusObject(StatusObject::DEAD);
				return true;
			}

			m_velocity = m_checkCollision->getVelocity();
			if (dir == DIR::BOTTOM)
			{
				// cập nhật cho luckybox
				LuckyBox* luckyBox = dynamic_cast<LuckyBox*>(gameObject);
				if (luckyBox != nullptr)
				{
					luckyBox->setMakeEffect(true);
					return true;
				}
					

				// cập nhật cho brick
				Brick* brick = dynamic_cast<Brick*>(gameObject);
				if (brick != nullptr)
				{
					if (!m_isBig && !m_canShoot)
					{
						brick->setMakeEffect(true);	
					}
					else
					{
						brick->setStatusObject(StatusObject::DEAD);
						brick->setIsBreak(true);
					}
					return true;
				}
			}
			else if (dir == DIR::TOP)
			{
				if (getFSM() == FSM_Mario::FALL || getFSM() == FSM_Mario::RUN) // fall gặp vật cản
					setLocation(Location::LOC_ON_GROUND);
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


	if (m_velocity.x >= maxVelocity.x)
		m_velocity.x = maxVelocity.x;
	else if (m_velocity.x <= minVelocity.x)
		m_velocity.x = minVelocity.x;

	if (m_velocity.y >= maxVelocity.y)
		m_velocity.y = maxVelocity.y;
	else if (m_velocity.y <= minVelocity.y)
		m_velocity.y = minVelocity.y;
	
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

Box Mario::getCamera()
{
	Box camera(m_worldPosition.x, m_worldPosition.y - VIEW_PORT_Y, SCREEN_WIDTH, SCREEN_HEIGHT);
	return camera;
}
