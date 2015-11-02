#pragma once
#include "LuckyBox.h"
#include "Brick.h"

#pragma region Base animation of item inbox
////////////// base item move up
class ItemInLuckyBoxIdle : public State<ItemInBox>
{
	static ItemInLuckyBoxIdle*	m_instance;
public:
	ItemInLuckyBoxIdle(){}
	virtual ~ItemInLuckyBoxIdle(){}

	static ItemInLuckyBoxIdle*		getInstance();

	virtual void		enter(ItemInBox* item);
	virtual void		execute(ItemInBox* item);
	virtual void		exit(ItemInBox* item);
};

///////////// base item show score
class ItemInLuckyBoxScore : public State<ItemInBox>
{
	static ItemInLuckyBoxScore*	m_instance;
public:
	ItemInLuckyBoxScore(){}
	virtual ~ItemInLuckyBoxScore(){}

	static ItemInLuckyBoxScore*		getInstance();

	virtual void		enter(ItemInBox* item);
	virtual void		execute(ItemInBox* item);
	virtual void		exit(ItemInBox* item);
};
#pragma endregion


#pragma region Animation of Lucky Box
//////////////////////////////////// animation for lucky box

class IdleLuckyBox : public State<LuckyBox>
{
	static IdleLuckyBox*	m_instance;
public:
	IdleLuckyBox(){}
	virtual ~IdleLuckyBox(){}

	static IdleLuckyBox*	getInstance();

	virtual void		enter(LuckyBox* item);
	virtual void		execute(LuckyBox* item);
	virtual void		exit(LuckyBox* item);

};

class LuckyBoxEffect : public State<LuckyBox>
{
	static LuckyBoxEffect*	m_instance;
public:
	LuckyBoxEffect(){}
	virtual ~LuckyBoxEffect(){}

	static LuckyBoxEffect*	getInstance();

	virtual void		enter(LuckyBox* item);
	virtual void		execute(LuckyBox* item);
	virtual void		exit(LuckyBox* item);

};

#pragma endregion


//////////////////////////////////////////// Flower gun
class FlowerGun : public State<ItemInBox>
{
	static FlowerGun*	m_instance;
public:
	FlowerGun(){}
	virtual ~FlowerGun(){}

	static FlowerGun*		getInstance();

	virtual void		enter(ItemInBox* item);
	virtual void		execute(ItemInBox* item);
	virtual void		exit(ItemInBox* item);
};


/////////////////////////////////////////// Mushroom item
class MoveMushroomItem : public State<ItemInBox>
{
	static MoveMushroomItem*	m_instance;
public:
	MoveMushroomItem(){}
	virtual ~MoveMushroomItem(){}

	static MoveMushroomItem*		getInstance();

	virtual void		enter(ItemInBox* item);
	virtual void		execute(ItemInBox* item);
	virtual void		exit(ItemInBox* item);
};


/////////////////////////////////////// Coin in lucky box
class CoinInLuckyBox : public State<ItemInBox>
{
	static CoinInLuckyBox*	m_instance;
public:
	CoinInLuckyBox(){}
	virtual ~CoinInLuckyBox(){}

	static CoinInLuckyBox*		getInstance();

	virtual void		enter(ItemInBox* item);
	virtual void		execute(ItemInBox* item);
	virtual void		exit(ItemInBox* item);
};


/////////////////////////////////// Item star
class StarItem : public State<ItemInBox>
{
	static StarItem*	m_instance;
public:
	float alpha;
	float v;

	StarItem(){}
	virtual ~StarItem(){}

	static StarItem*	getInstance();

	virtual void		enter(ItemInBox* item);
	virtual void		execute(ItemInBox* item);
	virtual void		exit(ItemInBox* item);
};


#pragma region Animation of brick
////////////// brick idle
class BrickIdle : public State<Brick>
{
	static BrickIdle*	m_instance;
public:
	BrickIdle(){}
	virtual ~BrickIdle(){}

	static BrickIdle*		getInstance();

	virtual void		enter(Brick* item);
	virtual void		execute(Brick* item);
	virtual void		exit(Brick* item);
};

///////////// brick effect move up
class BrickEffect : public State<Brick>
{
	static BrickEffect*	m_instance;
public:
	BrickEffect(){}
	virtual ~BrickEffect(){}

	static BrickEffect*		getInstance();

	virtual void		enter(Brick* item);
	virtual void		execute(Brick* item);
	virtual void		exit(Brick* item);
};

/////////////// brick broken
class BrickBroken : public State<Brick>
{
	static BrickBroken*	m_instance;
public:
	BrickBroken(){}
	virtual ~BrickBroken(){}

	static BrickBroken*		getInstance();

	virtual void		enter(Brick* item);
	virtual void		execute(Brick* item);
	virtual void		exit(Brick* item);
};
#pragma endregion