#pragma once
#include "stdafx.h"
#include "Sprite.h"
#include "Object.h"
#include "KeyBoard.h"
#include <vector>
#include <fstream>
using namespace std;


class MapObject : public Object
{
protected:
	CKeyBoard *m_keyboard;

	int Line = 0; // line to read
	vector<ObjectTittle> m_tileMapNode;
public:
	MapObject();
	virtual void init(CKeyBoard *keyboard, char *FileMap);
	virtual ~MapObject();
};

