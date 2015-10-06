#include "Mario.h"
#include "Source.h"
#include "Global.h"
#include <fstream>
#include "MarioOwnedState.h"

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


	m_Sprite = Source::getInstance()->getSprite(IDImage::IMG_MARIOSHEET);
	m_MaxVelocity = Vector2(3.00f, 10.0f);
	m_MinVelocity = Vector2(-3.00f, -10.0f);

	m_Gametime = new GameTime();
	m_Gametime->setTime();

	m_WorldPosition = Vector2(0, VIEW_PORT_Y);
	m_lives = 3;
	m_isBig = false;
	m_canShoot = false;
	m_isStar = false;

	m_stateMachine = new StateMachine<Mario>(this);
	m_stateMachine->changeState(Falling::getInstance());

	m_statusStateMachine = new StateMachine<Mario>(this);
	m_statusStateMachine->changeState(Small::getInstance());
}

void Mario::draw(LPD3DXSPRITE SpriteHandler)
{
	m_Sprite->setRect(frameList[m_currentFrame].rect);
	Object::draw(SpriteHandler);
}

void Mario::update()
{
	if (m_Gametime->getElapsedTime() < 1000 / 22) return;
	m_Gametime->update();
	
	if (m_FSM_Mario != FSM_Mario::EFFECT_BIG && m_FSM_Mario != FSM_Mario::EFFECT_FIRE && m_FSM_Mario != FSM_Mario::EFFECT_SMALL)
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
	m_Gametime->setTime();
	if (m_Gametime->getElapsedTime() < 1000 / 22)
		return;

	m_stateMachine->update();
	m_statusStateMachine->update();
}


void Mario::setVelocity(Vector2 velocity)
{
	m_Velocity = velocity;
	Vector2 maxVelocity = m_MaxVelocity;
	Vector2 minVelocity = m_MinVelocity;

	//if (!m_isBig && !m_canShoot) // update velocity for small
	//{
	//	maxVelocity.x--;
	//	maxVelocity.y--;
	//	minVelocity.x++;
	//	minVelocity.y++;
	//}

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
	delete m_Gametime;
	delete Falling::getInstance();
	delete Running::getInstance();
	delete Standing::getInstance();
	delete Jumping::getInstance();
	delete Sitting::getInstance();

	delete EffectBig::getInstance();
	delete EffectFire::getInstance();
	delete EffectSmall::getInstance();

	delete Star::getInstance();
	delete Small::getInstance();
	delete Dead::getInstance();
	delete Big::getInstance();
	delete Fire::getInstance();

	delete m_stateMachine;
	delete m_statusStateMachine;
}
