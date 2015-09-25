#pragma once

template <class entity_type>
class State
{
public:
	State(){}
	virtual			~State(){}

	virtual void	enter(entity_type*) = 0;
	virtual void	execute(entity_type*) = 0;
	virtual void	exit(entity_type*) = 0;
};