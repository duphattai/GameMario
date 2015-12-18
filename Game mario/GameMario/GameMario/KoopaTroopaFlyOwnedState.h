#pragma once
#include "Enemy.h"
#include <vector>
using namespace std;


Vector2 vanTocNemXien(int time, float v, float alpha);

class KoopaTroopaFlyMoveFly : public State<Enemy>// bình thường
{
	static KoopaTroopaFlyMoveFly*	m_instance;
	vector<Frame>			m_frameAnimation;
public:
	KoopaTroopaFlyMoveFly();
	virtual ~KoopaTroopaFlyMoveFly(){}

	static KoopaTroopaFlyMoveFly*		getInstance();

	virtual void		enter(Enemy* item);
	virtual void		execute(Enemy* item);
	virtual void		exit(Enemy* item);
};
