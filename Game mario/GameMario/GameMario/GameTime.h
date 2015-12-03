#pragma once
#include <Windows.h>
class GameTime
{
	DWORD m_PrevTime;
	DWORD m_CurrenTime;
	DWORD m_ElapsedTimeSecond;

	DWORD m_SumTime;

	static GameTime* m_instance;
	GameTime();
public:
	static GameTime* getInstance();
	void update();
	void setTime();

	void setSumTime(DWORD time){ m_SumTime = time; }
	DWORD getSumTime(){ return m_SumTime; }

	DWORD getElapsedTime()
	{
		return (m_CurrenTime - m_PrevTime);
	}
	~GameTime();
};

