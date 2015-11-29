#pragma once
#include "GameObject.h"
#include <vector>
using namespace std;

class Coin : public GameObject
{
	bool						m_finishAnimation;

	int							m_currentFrame;
	vector<Frame>				m_frameList;
public:
	Coin();
	~Coin();

	void					update();
	void					updateVelocity();
	void					draw(LPD3DXSPRITE SpriteHandler);

	virtual Box				getBouding();
};
