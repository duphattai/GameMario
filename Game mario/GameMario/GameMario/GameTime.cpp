#include "GameTime.h"

GameTime* GameTime::m_instance = 0;

GameTime* GameTime::getInstance()
{
	if (m_instance == NULL)
		m_instance = new GameTime();
	return m_instance;
}

GameTime::GameTime()
{
	m_CurrenTime = 0;
	m_PrevTime = 0;
	m_ElapsedTimeSecond = 0;
	m_SumTime = 0;
}

void GameTime::setTime()
{
	m_CurrenTime = GetTickCount();
}

void GameTime::update()
{
	m_SumTime += m_CurrenTime - m_PrevTime;
	m_PrevTime = m_CurrenTime;
}

GameTime::~GameTime()
{
}
