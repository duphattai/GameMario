#pragma once
#include "MapObject.h"
#include "Quadtree.h"
class Map1 : public MapObject
{
public:
	Map1(Mario *mario);
	void init(CKeyBoard *keyboard);
	void update();
	~Map1();
};

