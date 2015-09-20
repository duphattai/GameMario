#include "GameTime.h"


GameTime::GameTime()
{
	m_CurrenTime = 0;
	m_PrevTime = (float)GetTickCount() / 1000;
	m_ElapsedTimeSecond = 0;
}

void GameTime::setTime()
{
	m_CurrenTime = (float)GetTickCount() / 1000;
}

void GameTime::update()
{
	m_PrevTime = m_CurrenTime;
}

GameTime::~GameTime()
{
}
