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
#include "StateMachine.h"
using namespace std;


class MapObject : public GameObject
{
protected:
	StateMachine<MapObject>*			m_stateMachine;

	D3DXCOLOR							m_colorBackGround;

	GameObject*							createGameObject(ObjectTittle object);
	map<int, vector<ObjectTittle>>		readQuadTreeFromFile(TiXmlElement *nodeParent);
	void								buildQuadTree(map<int, vector<ObjectTittle>>);

	void								updateVelocity(Mario*);
public:
	MapObject();
	virtual ~MapObject();
	
	// xây dựng quadtree
	virtual void						init(IDMap);


	virtual	vector<GameObject*>			getListObjectOnCamera();
	D3DXCOLOR							getColorBackGround(){ return m_colorBackGround; }
	
	StateMachine<MapObject>*			getStateMachine(){ return m_stateMachine; }

	void								draw(LPD3DXSPRITE SpriteHandler);
	
	void								update();
};

