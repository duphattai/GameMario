#pragma once
#include "MapObject.h"
#include "Quadtree.h"
class Map1 : public MapObject
{
public:
	Map1();
	void init();
	void update();
	~Map1();
};

