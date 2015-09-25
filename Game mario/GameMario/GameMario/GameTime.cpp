#include "GameTime.h"


GameTime::GameTime()
{
	m_CurrenTime = 0;
	m_PrevTime = GetTickCount();
	m_ElapsedTimeSecond = 0;
}

void GameTime::setTime()
{
	m_CurrenTime = GetTickCount();
}

void GameTime::update()
{
	m_PrevTime = m_CurrenTime;
}

GameTime::~GameTime()
{
}
