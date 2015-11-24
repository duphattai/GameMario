#pragma once
#include "MapObject.h"


class BrosTitle : public State < MapObject >
{
private:
	static BrosTitle*	m_instance;
public:
	BrosTitle(){}
	virtual ~BrosTitle(){}

	static BrosTitle*	getInstance();

	virtual void		enter(MapObject*);
	virtual void		execute(MapObject*);
	virtual void		exit(MapObject*);
	virtual void		draw(MapObject*, LPD3DXSPRITE);
};


class MapOne : public State < MapObject >
{
private:
	static MapOne*	m_instance;
public:
	MapOne(){}
	virtual ~MapOne(){}

	static MapOne*	getInstance();

	virtual void		enter(MapObject*);
	virtual void		execute(MapObject*);
	virtual void		exit(MapObject*);
};