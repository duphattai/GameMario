#include "Source.h"

Source* Source::m_Instance = NULL;

Source::Source()
{}

void Source::init(LPDIRECT3DDEVICE9 d3ddv)
{
	m_Map1 = new Sprite();
	m_Map1->loadImageFromPath(d3ddv, MapData_1, 44, 44, D3DCOLOR_XRGB(255, 0, 255));

	m_MarioSheet = new Sprite();
	m_MarioSheet->loadImageFromPath(d3ddv, L"Resources//mario.png", D3DCOLOR_XRGB(255, 0, 255));
}

Sprite* Source::getSprite(int Id)
{
	switch (Id)
	{
		case IDImage::IMG_MAP1:
			return m_Map1;
		case IDImage::IMG_MARIOSHEET:
			return m_MarioSheet;
	}
}

Source* Source::getInstance()
{
	if (m_Instance == NULL)
		m_Instance = new Source();

	return m_Instance;
}

void Source::clear()
{
	delete m_Map1;
	delete m_MarioSheet;
}

Source::~Source()
{
}
