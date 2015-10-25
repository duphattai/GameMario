#pragma once
#include "ItemInBox.h"
#include "LuckyBox.h"


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

//////////////////////////////////////////////



class Gun : public State<ItemInBox>
{
	static Gun*	m_instance;
public:
	Gun(){}
	virtual ~Gun(){}

	static Gun*		getInstance();

	virtual void		enter(ItemInBox* item);
	virtual void		execute(ItemInBox* item);
	virtual void		exit(ItemInBox* item);
};

