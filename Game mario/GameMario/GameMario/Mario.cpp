#include "Mario.h"
#include "ReSource.h"
#include "Global.h"
#include <fstream>
#include "MarioOwnedState.h"
#include "LuckyBox.h"

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


	m_Sprite = ReSource::getInstance()->getSprite(IDImage::IMG_MARIOSHEET);
	m_MaxVelocity = Vector2(3.00f, 10.0f);
	m_MinVelocity = Vector2(-3.00f, -10.0f);


	m_WorldPosition = Vector2(0, VIEW_PORT_Y);
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
}

void Mario::draw(LPD3DXSPRITE SpriteHandler)
{
	m_Sprite->setRect(frameList[m_currentFrame].rect);
	GameObject::draw(SpriteHandler);
}

void Mario::update()
{
	// không hiệu ứng small, fire, big mới update position
	if (!m_effectBig && !m_effectFire && !m_effectSmall)
	{
		m_Position.x += m_Velocity.x;
		m_Position.y += m_Velocity.y;
	}
	

	// make mario not move off camera
	if (m_Position.x < m_WorldPosition.x)
		m_Position.x = m_WorldPosition.x;

	// update camera just move right
	if (m_WorldPosition.x < m_Position.x - SCREEN_WIDTH / 2)
		m_WorldPosition.x = m_Position.x - SCREEN_WIDTH / 2;
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

	//hard code
	m_Box.x += 6;
	m_Box.width = 4;

	return m_Box;
}

bool Mario::isCollision(GameObject* gameObject)
{
	int type = gameObject->getTypeObject();
	if (type == TypeObject::Dynamic_TiledMap) // không xét va cham với tiled map 
		return false;

	DIR dir = m_checkCollision->isCollision(this, gameObject);
	if (dir != DIR::NONE)
	{
		m_Velocity = m_checkCollision->getVelocity();
		if (type == TypeObject::Dynamic_StandPosition)
		{
			if ((getFSM() == FSM_Mario::FALL || getFSM() == FSM_Mario::RUN) && dir == DIR::TOP) // fall gặp vật cản
					setLocation(Location::LOC_ON_GROUND);

			if (getDirCollision() == DIR::NONE)
				setDirCollision(dir);
		}
		else if (type == TypeObject::Moving_Enemy)
		{

		}
		else if (type == TypeObject::Dynamic_Item)
		{
			if (getDirCollision() == DIR::NONE)
				setDirCollision(dir);

			if (dir == DIR::BOTTOM)
			{
				LuckyBox* luckyBox = static_cast<LuckyBox*>(gameObject);
				luckyBox->setMakeEffect(true);

				setLocation(Location::LOC_IN_AIR);
			}
			else if (dir == DIR::TOP)
			{
				if ((getFSM() == FSM_Mario::FALL || getFSM() == FSM_Mario::RUN) && dir == DIR::TOP) // fall gặp vật cản
					setLocation(Location::LOC_ON_GROUND);
			}
		}

		return true;
	}

	return false;
}

void Mario::setVelocity(Vector2 velocity)
{
	m_Velocity = velocity;
	Vector2 maxVelocity = m_MaxVelocity;
	Vector2 minVelocity = m_MinVelocity;


	if (m_Velocity.x >= maxVelocity.x)
		m_Velocity.x = maxVelocity.x;
	else if (m_Velocity.x <= minVelocity.x)
		m_Velocity.x = minVelocity.x;

	if (m_Velocity.y >= maxVelocity.y)
		m_Velocity.y = maxVelocity.y;
	else if (m_Velocity.y <= minVelocity.y)
		m_Velocity.y = minVelocity.y;
	
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
