#pragma once
#include "Sprite.h"
class ReSource
{
	Sprite *m_TileMap;
	Sprite *m_MarioSheet;
	Sprite *m_ItemSheet;

	ReSource();
	static ReSource *m_Instance;
public:
	static ReSource* getInstance();
	void init(LPDIRECT3DDEVICE9 d3ddv);
	Sprite *getSprite(int IdImage);
	void clear();
	~ReSource();
};

