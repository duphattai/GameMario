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
	delete AutoAnimation::getInstance();


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
	m_isDead = false;
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
	// Không hiệu ứng small, fire, big mới update position
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
	if (!m_effectBig && !m_effectFire && !m_effectSmall && !m_isDead)
		m_stateMachine->update();
	
	m_statusStateMachine->update();
	
	// 24/10 
	// Set default, dùng để xét va chạm di chuyển trên map
	setDirCollision(DIR::NONE);
	setLocation(Location::LOC_IN_AIR);
}

Box Mario::getBouding()
{
	GameObject::getBouding();	
	return m_box;
}

bool Mario::isCollision(GameObject* gameObject)
{
	int type = gameObject->getTypeObject();
	if (type == TypeObject::Dynamic_TiledMap   // Tiled map 
		|| gameObject->getStatusOBject() == StatusObject::DEAD // Chết
		|| !gameObject->isActive() // Chưa active
		|| m_stateMachine->isInState(*AutoAnimation::getInstance())
		|| m_status == StatusObject::DEAD) 
		return false;


	// Stand position
	DIR dir = Collision::getInstance()->isCollision(this, gameObject);
	if (dir != DIR::NONE)
	{
		// Stand position
		if (type == TypeObject::Dynamic_StandPosition)
		{
			m_velocity = Collision::getInstance()->getVelocity();
			if ((getFSM() == FSM_Mario::FALL || getFSM() == FSM_Mario::RUN) && dir == DIR::TOP) // fall gặp vật cản
					setLocation(Location::LOC_ON_GROUND);

			if (getDirCollision() == DIR::NONE)
				setDirCollision(dir);
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

	// Thiết lập max, min cho trường hợp small
	/*if (!m_isBig && !m_canShoot)
	{
		maxVelocity.x--;
		minVelocity.y++;
	}*/

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
	m_width = abs(frameList[m_currentFrame].rect.left - frameList[m_currentFrame].rect.right) - 6;
	m_height = abs(frameList[m_currentFrame].rect.top - frameList[m_currentFrame].rect.bottom) - 2;
}
