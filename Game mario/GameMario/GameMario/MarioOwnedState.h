#pragma once

#include "State.h"

class Mario;

#pragma region action mario
////////////////////////////////////////////////////////////////////
//STANDING
class Standing : public State < Mario >
{
private:
	static Standing*	m_instance;
public:
	Standing(){}
	virtual ~Standing(){}

	static Standing*	getInstance();

	virtual void		enter(Mario* mario);
	virtual void		execute(Mario* mario);
	virtual void		exit(Mario* mario);
};
////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////
//RUNNING
class Running : public State < Mario >
{
private:
	static Running*	m_instance;
public:
	Running(){}
	virtual ~Running(){}

	static Running*	getInstance();

	virtual void		enter(Mario* mario);
	virtual void		execute(Mario* mario);
	virtual void		exit(Mario* mario);
};
////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////
//SITTING
class Sitting : public State < Mario >
{
private:
	static Sitting*	m_instance;
public:
	Sitting(){}
	virtual ~Sitting(){}

	static Sitting*	getInstance();

	virtual void		enter(Mario* mario);
	virtual void		execute(Mario* mario);
	virtual void		exit(Mario* mario);
};
////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////
//JUMPING
class Jumping : public State < Mario >
{
private:
	static Jumping*	m_instance;
	int				m_timeJump;
public:
	Jumping(){}
	virtual ~Jumping(){}

	static Jumping*	getInstance();

	virtual void		enter(Mario* mario);
	virtual void		execute(Mario* mario);
	virtual void		exit(Mario* mario);
};
////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////
//FALLING
class Falling : public State < Mario >
{
private:
	static Falling*	m_instance;
public:
	Falling(){}
	virtual ~Falling(){}

	static Falling*	getInstance();

	virtual void		enter(Mario* mario);
	virtual void		execute(Mario* mario);
	virtual void		exit(Mario* mario);
};
////////////////////////////////////////////////////////////////////

#pragma endregion


#pragma region status of mario
////////////////////////////////////////////////////////////////////// STATUS OF MARIO /////////////////////////////////////////
////Small
class Small : public State < Mario >
{
private:
	static Small*	m_instance;
public:
	Small();
	virtual ~Small(){}

	static Small*	getInstance();

	virtual void		enter(Mario* mario);
	virtual void		execute(Mario* mario);
	virtual void		exit(Mario* mario);
};

// BIG
class Big : public State < Mario >
{
private:
	static Big*	m_instance;
public:
	Big();
	virtual ~Big(){}

	static Big*	getInstance();

	virtual void		enter(Mario* mario);
	virtual void		execute(Mario* mario);
	virtual void		exit(Mario* mario);
};
// Fire
class Fire : public State < Mario >
{
private:
	static Fire*	m_instance;
public:
	Fire();
	virtual ~Fire(){}

	static Fire*	getInstance();

	virtual void		enter(Mario* mario);
	virtual void		execute(Mario* mario);
	virtual void		exit(Mario* mario);
};

// Dead
class Dead : public State < Mario >
{
private:
	static Dead*	m_instance;
public:
	Dead();
	virtual ~Dead(){}

	static Dead*	getInstance();

	virtual void		enter(Mario* mario);
	virtual void		execute(Mario* mario);
	virtual void		exit(Mario* mario);
};

////EFFECT BIG
class EffectBig : public State < Mario >
{
private:
	static EffectBig*	m_instance;
	vector<int>				m_frameAnimation;
	int						m_currentIndex;
public:
	EffectBig();
	virtual ~EffectBig(){}

	static EffectBig*	getInstance();

	virtual void		enter(Mario* mario);
	virtual void		execute(Mario* mario);
	virtual void		exit(Mario* mario);
};


////EFFECT SMALL
class EffectSmall : public State < Mario >
{
private:
	static EffectSmall*	m_instance;
	vector<int>				m_frameAnimation;
	int						m_currentIndex;
public:
	EffectSmall();
	virtual ~EffectSmall(){}

	static EffectSmall*	getInstance();

	virtual void		enter(Mario* mario);
	virtual void		execute(Mario* mario);
	virtual void		exit(Mario* mario);
};

/////////////////////////////////////////
/////EFFECT FIRE
class EffectFire : public State < Mario >
{
private:
	static EffectFire*	m_instance;
	vector<int>				m_frameAnimation;
	int						m_currentIndex;
public:
	EffectFire();
	virtual ~EffectFire(){}

	static EffectFire*	getInstance();

	virtual void		enter(Mario* mario);
	virtual void		execute(Mario* mario);
	virtual void		exit(Mario* mario);
};

#pragma endregion