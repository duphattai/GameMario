#pragma once
#include "Enemy.h"


class GoombaDieByGun : public State<Enemy>
{
	static GoombaDieByGun*	m_instance;
public:
	GoombaDieByGun(){}
	virtual ~GoombaDieByGun(){}

	static GoombaDieByGun*		getInstance();

	virtual void		enter(Enemy* item);
	virtual void		execute(Enemy* item);
	virtual void		exit(Enemy* item);
};


class GoombaMove : public State<Enemy>
{
	static GoombaMove*	m_instance;
public:
	GoombaMove(){}
	virtual ~GoombaMove(){}

	static GoombaMove*		getInstance();

	virtual void		enter(Enemy* item);
	virtual void		execute(Enemy* item);
	virtual void		exit(Enemy* item);
};

class GoombaDieByJump : public State<Enemy>
{
	static GoombaDieByJump*	m_instance;
public:
	GoombaDieByJump(){}
	virtual ~GoombaDieByJump(){}

	static GoombaDieByJump*		getInstance();

	virtual void		enter(Enemy* item);
	virtual void		execute(Enemy* item);
	virtual void		exit(Enemy* item);
};

