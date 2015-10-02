#pragma once

#include "State.h"

class Mario;

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


//////////////////////////////////////////////////////////////////////
////RUNJUMPING
//class RunJumping : public State < Mario >
//{
//private:
//	static RunJumping*	m_instance;
//public:
//	RunJumping(){}
//	virtual ~RunJumping(){}
//
//	static RunJumping*	getInstance();
//
//	virtual void		enter(Mario* mario);
//	virtual void		execute(Mario* mario);
//	virtual void		exit(Mario* mario);
//};
//////////////////////////////////////////////////////////////////////