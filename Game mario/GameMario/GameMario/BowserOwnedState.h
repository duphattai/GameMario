#pragma once
#include "State.h"
#include "Enemy.h"

class BowserDieByGun : public State<Enemy>
{
	static BowserDieByGun*	m_instance;
public:
	BowserDieByGun(){}
	virtual ~BowserDieByGun(){}

	static BowserDieByGun*		getInstance();

	virtual void		enter(Enemy* item);
	virtual void		execute(Enemy* item);
	virtual void		exit(Enemy* item);
};

class BowserMove : public State<Enemy>
{
	static BowserMove*	m_instance;
	vector<Frame>		m_frameAnimation;
public:
	BowserMove();
	virtual ~BowserMove(){};

	static BowserMove*		getInstance();

	virtual void		enter(Enemy* item);
	virtual void		execute(Enemy* item);
	virtual void		exit(Enemy* item);
};

class BowserAttackByFire : public State<Enemy>
{
	static BowserAttackByFire*	m_instance;
	vector<Frame>				m_frameAnimation;
public:
	BowserAttackByFire();
	virtual ~BowserAttackByFire(){};

	static BowserAttackByFire*		getInstance();

	virtual void		enter(Enemy* item);
	virtual void		execute(Enemy* item);
	virtual void		exit(Enemy* item);
};

class BowserAttackByHammer : public State<Enemy>
{
	static BowserAttackByHammer*	m_instance;
	vector<Frame>					m_frameAnimation;
public:
	BowserAttackByHammer();
	virtual ~BowserAttackByHammer(){}

	static BowserAttackByHammer*		getInstance();

	virtual void		enter(Enemy* item);
	virtual void		execute(Enemy* item);
	virtual void		exit(Enemy* item);
};

class BowserJump : public State<Enemy>
{
	static BowserJump*	m_instance;
public:
	BowserJump(){};
	virtual ~BowserJump(){};

	static BowserJump*		getInstance();

	virtual void		enter(Enemy* item);
	virtual void		execute(Enemy* item);
	virtual void		exit(Enemy* item);
};