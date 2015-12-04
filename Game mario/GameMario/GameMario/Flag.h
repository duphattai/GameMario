#pragma once
#include "GameObject.h"
#include <vector>
using namespace std;
class Flag : public GameObject
{
	bool					m_finishAnimation;
	bool					m_makeEffect;

	Vector2					m_flagPosition; // tọa lá cờ

	vector<Frame>			m_frameList;

	void					setPosition(float, float){};
public:
	Flag(Vector2);
	~Flag();

	void					setMakeEffect(bool x){ m_makeEffect = x; }

	void					update();
	void					updateVelocity();
	void					draw(LPD3DXSPRITE SpriteHandler);

	virtual Box				getBouding();
};

