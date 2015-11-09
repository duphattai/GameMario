#pragma once
#include "Bullet.h"

Vector2 vanTocNemXien(int time, float v, float alpha);

class BulletIdle : public State<Bullet>
{
	static BulletIdle*	m_instance;
public:
	BulletIdle(){}
	virtual ~BulletIdle(){}

	static BulletIdle*	getInstance();

	virtual void		enter(Bullet* bullet);
	virtual void		execute(Bullet* bullet);
	virtual void		exit(Bullet* bullet);
};

class BulletMoving : public State<Bullet>
{
	static BulletMoving*	m_instance;
public:
	BulletMoving(){}
	virtual ~BulletMoving(){}

	static BulletMoving*	getInstance();

	virtual void		enter(Bullet* enemy);
	virtual void		execute(Bullet* enemy);
	virtual void		exit(Bullet* enemy);
};

class BulletExplode : public State<Bullet>
{
	static BulletExplode*	m_instance;
public:
	BulletExplode(){}
	virtual ~BulletExplode(){}

	static BulletExplode*	getInstance();

	virtual void		enter(Bullet* enemy);
	virtual void		execute(Bullet* enemy);
	virtual void		exit(Bullet* enemy);
};