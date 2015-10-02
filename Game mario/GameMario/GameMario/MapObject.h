#pragma once
#include "stdafx.h"
#include "Sprite.h"
#include "Object.h"
#include "KeyBoard.h"
#include <vector>
#include <fstream>
#include "Mario.h";
using namespace std;


class MapObject : public Object
{
protected:
	CKeyBoard *m_keyboard;
	Mario	  *m_Mario;

	int Line = 0; // line to read
	vector<ObjectTittle> m_tileMapNode;
	vector<Box>	m_StandPositionOnMap;

	vector<Box> getTileNodeOnScreen();
public:
	MapObject();
	virtual void init(CKeyBoard *keyboard, char *FileMap);
	virtual ~MapObject();
};

