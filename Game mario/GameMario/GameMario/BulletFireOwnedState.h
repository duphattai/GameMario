#pragma once
#include "BulletFire.h"


class BulletFireIdle : public State<BulletFire>
{
	static BulletFireIdle*	m_instance;
public:
	BulletFireIdle(){}
	virtual ~BulletFireIdle(){}

	static BulletFireIdle*	getInstance();

	virtual void		enter(BulletFire* bullet);
	virtual void		execute(BulletFire* bullet);
	virtual void		exit(BulletFire* bullet);
};

class BulletFireMoving : public State<BulletFire>
{
	static BulletFireMoving*	m_instance;
public:
	BulletFireMoving(){}
	virtual ~BulletFireMoving(){}

	static BulletFireMoving*	getInstance();

	virtual void		enter(BulletFire* enemy);
	virtual void		execute(BulletFire* enemy);
	virtual void		exit(BulletFire* enemy);
};

class BulletFireExplode : public State<BulletFire>
{
	static BulletFireExplode*	m_instance;
public:
	BulletFireExplode(){}
	virtual ~BulletFireExplode(){}

	static BulletFireExplode*	getInstance();

	virtual void		enter(BulletFire* enemy);
	virtual void		execute(BulletFire* enemy);
	virtual void		exit(BulletFire* enemy);
};
