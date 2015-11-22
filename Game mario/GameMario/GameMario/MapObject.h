#pragma once
#include "stdafx.h"
#include "Sprite.h"
#include "KeyBoard.h"
#include <vector>
#include <fstream>
#include <map>
#include "Mario.h";
#include "Quadtree.h"
#include "Xml\tinyxml.h"
using namespace std;


class MapObject : public GameObject
{
protected:
	map<int, vector<ObjectTittle>>		m_quadtreeNode;
	Quadtree*							m_quadTree;
	D3DXCOLOR							m_colorBackGround;

	GameObject*							createGameObject(ObjectTittle object);
	map<int, vector<ObjectTittle>>		readQuadTreeFromFile(TiXmlElement *nodeParent);
	void								buildQuadTree();
public:
	MapObject();
	virtual void						init(char *FileMap);
	virtual	vector<GameObject*>			getListObjectOnCamera();
	D3DXCOLOR							getColorBackGround(){ return m_colorBackGround; }
	void								draw(LPD3DXSPRITE SpriteHandler);

	void								update(Mario*);

	Quadtree*							getTree(){ return m_quadTree; }

	virtual ~MapObject();
};

