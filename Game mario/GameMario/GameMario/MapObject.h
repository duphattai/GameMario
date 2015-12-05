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
	Vector2								m_checkPoint; // tọa độ nơi nếu mario đi vượt quá sẽ hồi sinh hoặc trở lại từ đầu
	IDMap								m_idMap;
	bool								m_isScrollMap;


	GameObject*							createGameObject(ObjectTittle object);
	map<int, vector<ObjectTittle>>		readQuadTreeFromFile(char*);
	void								buildQuadTree(map<int, vector<ObjectTittle>>);
public:
	MapObject();
	virtual ~MapObject();
	
	// xây dựng quadtree, tự hủy cây
	virtual void						init(IDMap);

	virtual	vector<GameObject*>			getListObjectOnCamera();

	void								setColorBackGround(D3DXCOLOR color){ m_colorBackGround = color; }
	D3DXCOLOR							getColorBackGround(){ return m_colorBackGround; }
	
	StateMachine<MapObject>*			getStateMachine(){ return m_stateMachine; }

	void								draw(LPD3DXSPRITE SpriteHandler);
	void								updateVelocity();
	void								update();

	bool								isScrollMap(){ return m_isScrollMap; }
	void								setScrollMap(bool x){ m_isScrollMap = x; }

	void								setCheckPoin(Vector2 checkPoint){ m_checkPoint = checkPoint; }
	Vector2								getCheckPoint(){ return m_checkPoint; }
};

