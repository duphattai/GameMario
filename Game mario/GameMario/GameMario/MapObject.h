#pragma once
#include "stdafx.h"
#include "Sprite.h"
#include "KeyBoard.h"
#include <vector>
#include <fstream>
#include <map>
#include "Mario.h";
#include "Quadtree.h"
using namespace std;


class MapObject : public GameObject
{
protected:
	CKeyBoard *m_keyboard;
	Mario	  *m_Mario;

	map<int, vector<ObjectTittle>> m_quadtreeNode;
	Quadtree*	m_quadTree;
	D3DXCOLOR	m_colorBackGround;
public:
	MapObject();
	virtual void	init(CKeyBoard *keyboard, char *FileMap);
	virtual			vector<GameObject*>		getListObjectOnCamera();
	D3DXCOLOR		getColorBackGround(){ return m_colorBackGround; }
	virtual ~MapObject();
};

