#pragma once
#include "GameObject.h"
#include <map>
#include <vector>

class Quadtree
{
private:
	Box m_Rect;
	vector<GameObject*> m_listObject;

	Quadtree* m_AreaOne; // x > 0, y > 0
	Quadtree* m_AreaTwo; // x < 0, y > 0
	Quadtree* m_AreaThree; // x < 0, y < 0
	Quadtree* m_AreaFour;  // x > 0, y < 0

	
	//void	insert(Quadtree*, GameObject*);

	bool	IsContain(Box);
public:
	Quadtree();
	~Quadtree();
	void					insert(vector<GameObject*>, int, Box);
	void					insert(GameObject*, int, Box);
	void					buildTree(map<int, vector<GameObject*>>, Box);
	void					update(vector<GameObject*>);
	void					remove(GameObject*);

	vector<GameObject*>		getListObjects(Box, vector<GameObject*> list);
	void					release();
};

