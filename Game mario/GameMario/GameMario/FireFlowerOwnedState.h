#pragma once
#include "State.h"
#include "Enemy.h"

class FireFlowerMove : public State<Enemy>// nấm lúc bình thường
{
	static FireFlowerMove*	m_instance;
public:
	FireFlowerMove(){}
	virtual ~FireFlowerMove(){}

	static FireFlowerMove*		getInstance();

	virtual void		enter(Enemy* item);
	virtual void		execute(Enemy* item);
	virtual void		exit(Enemy* item);
};


class FireFlowerDieByGun : public State<Enemy>
{
	static FireFlowerDieByGun*	m_instance;
public:
	FireFlowerDieByGun(){}
	virtual ~FireFlowerDieByGun(){}

	static FireFlowerDieByGun*		getInstance();

	virtual void		enter(Enemy* item);
	virtual void		execute(Enemy* item);
	virtual void		exit(Enemy* item);
};