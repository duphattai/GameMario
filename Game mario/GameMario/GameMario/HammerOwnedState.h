#pragma once
#include "Hammer.h"

Vector2 vanTocNemXien(int time, float v, float alpha);

class HammerIdle : public State<Hammer>
{
	static HammerIdle*	m_instance;
public:
	HammerIdle(){}
	virtual ~HammerIdle(){}

	static HammerIdle*	getInstance();

	virtual void		enter(Hammer* bullet);
	virtual void		execute(Hammer* bullet);
	virtual void		exit(Hammer* bullet);
};

class HammerMoving : public State<Hammer>
{
	static HammerMoving*	m_instance;
public:
	HammerMoving(){}
	virtual ~HammerMoving(){}

	static HammerMoving*	getInstance();

	virtual void		enter(Hammer* enemy);
	virtual void		execute(Hammer* enemy);
	virtual void		exit(Hammer* enemy);
};

class HammerExplode : public State<Hammer>
{
	static HammerExplode*	m_instance;
public:
	HammerExplode(){}
	virtual ~HammerExplode(){}

	static HammerExplode*	getInstance();

	virtual void		enter(Hammer* enemy);
	virtual void		execute(Hammer* enemy);
	virtual void		exit(Hammer* enemy);
};