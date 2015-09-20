#pragma once
#include <Windows.h>
class GameTime
{
	float m_PrevTime;
	float m_CurrenTime;

	float m_ElapsedTimeSecond;
public:
	GameTime();
	void update();
	void setTime();
	float getElapsedTimePerSecond()
	{
		return (m_CurrenTime - m_PrevTime);
	}
	~GameTime();
};

