#pragma once
#include "Enemy.h"
#include <vector>
using namespace std;

class KoopaTroopaMove : public State<Enemy>// bình thường
{
	static KoopaTroopaMove*	m_instance;
	vector<Frame>			m_frameAnimation;
public:
	KoopaTroopaMove();
	virtual ~KoopaTroopaMove(){}

	static KoopaTroopaMove*		getInstance();

	virtual void		enter(Enemy* item);
	virtual void		execute(Enemy* item);
	virtual void		exit(Enemy* item);
};

class KoopaTroopaShellNotMove : public State<Enemy>// mai rùa ko di chuyển
{
	static KoopaTroopaShellNotMove*	m_instance;
	vector<Frame>					m_frameAnimation;
public:
	KoopaTroopaShellNotMove();
	virtual ~KoopaTroopaShellNotMove(){}

	static KoopaTroopaShellNotMove*		getInstance();

	virtual void		enter(Enemy* item);
	virtual void		execute(Enemy* item);
	virtual void		exit(Enemy* item);
};

class KoopaTroopaShellMove : public State<Enemy>// mai rùa di chuyển
{
	static KoopaTroopaShellMove*	m_instance;
	vector<Frame>					m_frameAnimation;
public:
	KoopaTroopaShellMove();
	virtual ~KoopaTroopaShellMove(){}

	static KoopaTroopaShellMove*		getInstance();

	virtual void		enter(Enemy* item);
	virtual void		execute(Enemy* item);
	virtual void		exit(Enemy* item);
};

class KoopaTroopaDieByGun : public State<Enemy>// ko di chuyển
{
	static KoopaTroopaDieByGun*	m_instance;
public:
	KoopaTroopaDieByGun(){}
	virtual ~KoopaTroopaDieByGun(){}

	static KoopaTroopaDieByGun*		getInstance();

	virtual void		enter(Enemy* item);
	virtual void		execute(Enemy* item);
	virtual void		exit(Enemy* item);
};