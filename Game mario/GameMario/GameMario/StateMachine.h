#pragma once

#include "State.h"
#include <stdio.h>

template <class entity_type>
class StateMachine
{
private:
	entity_type*		m_owner;
	State<entity_type>*	m_current_state;

	State<entity_type>*	m_prevous_state;


public:
	StateMachine(entity_type* owner)
	{
		m_owner = owner;
		m_current_state = NULL;
		m_prevous_state = NULL;
	}
	virtual	~StateMachine()
	{
		m_owner = NULL;
		m_current_state = NULL;
		m_prevous_state = NULL;
	}

	void	setCurrentState(State<entity_type>* s){ m_current_state = s; }
	void	setPreviousState(State<entity_type>* s){ m_prevous_state = s; }

	void	update()
	{
		if (m_current_state)
			m_current_state->execute(m_owner);
	}
	void	changeState(State<entity_type>* new_state)
	{
		if (m_current_state)
		{
			m_prevous_state = m_current_state;
			m_current_state->exit(m_owner);
		}

		m_current_state = new_state;
		m_current_state->enter(m_owner);
	}
	void	revertToPreviousState(){ ChangeState(m_prevous_state); }
	bool	isInState(State<entity_type>& st){ return typeid(*m_current_state) == typeid(st); }

	State<entity_type>* GetCurrentState(){ return m_current_state; }
	State<entity_type>* GetPreviousState(){ return m_prevous_state; }
};