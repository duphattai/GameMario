#pragma once
#include "stdafx.h"


class Sprite
{
protected:
	LPDIRECT3DTEXTURE9 m_Image;

	int m_Width;
	int m_Height;
	int m_SpritePerRow;
	int m_Count;
	int m_Index;

	RECT m_Rect;
public:
	Sprite();
	Sprite(const Sprite &copy);
	void loadImageFromPath(LPDIRECT3DDEVICE9 d3ddv, char *Path, int spriteperrow, int count, D3DXCOLOR color);
	void next();
	void reset();
	virtual void draw(LPD3DXSPRITE SpriteHandler, D3DXVECTOR2 position, D3DXVECTOR2 scale, int vpx, int vpy);
	int getCount();

	int getWidth();
	int getHeight();
	int getCurrentIndex();
	void setIndex(int);

	~Sprite();
};

