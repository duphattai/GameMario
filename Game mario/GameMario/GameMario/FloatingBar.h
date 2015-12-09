#pragma once
#include "GameObject.h"
#include <vector>
using namespace std;

enum class FloatingBarMove
{
	MoveUp,
	MoveDown,
	MoveLeft,
	MoveRight
};


class FloatingBar : public GameObject
{
	FloatingBarMove				m_typeMove;

	int							m_currentFrame;
	vector<Frame>				m_frameList;
public:
	FloatingBar(FloatingBarMove);
	~FloatingBar();

	void					update();
	void					updateVelocity();
	void					draw(LPD3DXSPRITE SpriteHandler);

	FloatingBarMove			getTypeFloatingBar(){ return m_typeMove; }

	virtual Box				getBouding();
};

