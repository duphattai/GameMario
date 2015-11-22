#include "Map1.h"
#include "ReSource.h"
#include "SwapAABB.h"
#include "LuckyBox.h"
#include "Brick.h"

Map1::Map1()
{
	m_sprite = ReSource::getInstance()->getSprite(IDImage::IMG_TILEMAP);
	m_worldPosition.y = VIEW_PORT_Y;
}


void Map1::init()
{
	MapObject::init(NodeTileMap_1);
}

void Map1::update()
{
}


Map1::~Map1()
{
}
