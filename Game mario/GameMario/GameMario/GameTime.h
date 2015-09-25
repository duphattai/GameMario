#pragma once
#include <Windows.h>
class GameTime
{
	DWORD m_PrevTime;
	DWORD m_CurrenTime;

	DWORD m_ElapsedTimeSecond;
public:
	GameTime();
	void update();
	void setTime();
	DWORD getElapsedTimePerSecond()
	{
		return (m_CurrenTime - m_PrevTime);
	}
	~GameTime();
};

