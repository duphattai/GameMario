#pragma once
#include "MapObject.h"
class Map1 : public MapObject
{
public:
	Map1(Mario *mario);
	void init(CKeyBoard *keyboard);
	void draw(LPD3DXSPRITE SpriteHandler);
	void update();
	void updateCollision();
	~Map1();
};
