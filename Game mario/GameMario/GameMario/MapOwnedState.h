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

class ChangeMap : public State < MapObject >
{
private:
	static ChangeMap*	m_instance;
	int					m_countTime;
public:
	ChangeMap(){}
	virtual ~ChangeMap(){}

	static ChangeMap*	getInstance();

	virtual void		enter(MapObject*);
	virtual void		execute(MapObject*);
	virtual void		exit(MapObject*);
	virtual void		draw(MapObject*, LPD3DXSPRITE);
};



class MapOne : public State < MapObject >
{
private:
	Box m_boxGoInSubMap; // nơi vào submap từ map chính
	Box m_boxGoOutSubMap; // tọa độ map chính khi ra khỏi submap

	Box	m_boxStartSubMap; // tọa độ mario trong submap
	Box m_boxEndSubMap; // toa độ ra khỏi submap

	Box m_boxStartMap; // toa độ mario xuất hiện
	Box m_boxEndMap; // tọa độ kết thúc map

	static MapOne*	m_instance;
public:
	MapOne();
	virtual ~MapOne(){}

	static MapOne*	getInstance();

	virtual void		enter(MapObject*);
	virtual void		execute(MapObject*);
	virtual void		exit(MapObject*);
};

