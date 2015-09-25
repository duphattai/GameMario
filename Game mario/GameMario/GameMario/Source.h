#pragma once
#include "Sprite.h"
class Source
{
	Sprite *m_Map1;
	Sprite *m_MarioSheet;
	Source();
	static Source *m_Instance;
public:
	static Source* getInstance();
	void init(LPDIRECT3DDEVICE9 d3ddv);
	Sprite *getSprite(int IdImage);
	void clear();
	~Source();
};

