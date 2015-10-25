#include "ReSource.h"

ReSource* ReSource::m_Instance = NULL;

ReSource::ReSource()
{}

void ReSource::init(LPDIRECT3DDEVICE9 d3ddv)
{
	m_TileMap = new Sprite();
	m_TileMap->loadImageFromPath(d3ddv, TileMapPath, 33, 924, D3DCOLOR_XRGB(255, 0, 255));

	m_MarioSheet = new Sprite();
	m_MarioSheet->loadImageFromPath(d3ddv, L"Resources//mario.png", D3DCOLOR_XRGB(255, 0, 255));

	m_ItemSheet = new Sprite();
	m_ItemSheet->loadImageFromPath(d3ddv, L"Resources//items.png", D3DCOLOR_XRGB(255, 0, 255));
}

Sprite* ReSource::getSprite(int Id)
{
	switch (Id)
	{
		case IDImage::IMG_TILEMAP:
			return m_TileMap;
		case IDImage::IMG_MARIOSHEET:
			return m_MarioSheet;
		case IDImage::IMG_ITEMSHEET:
			return m_ItemSheet;
	}
}

ReSource* ReSource::getInstance()
{
	if (m_Instance == NULL)
		m_Instance = new ReSource();

	return m_Instance;
}

void ReSource::clear()
{
	delete m_TileMap;
	delete m_MarioSheet;
	delete m_ItemSheet;
}

ReSource::~ReSource()
{
}
