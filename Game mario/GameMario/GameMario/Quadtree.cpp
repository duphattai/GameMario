#include "Quadtree.h"
#include <string>

Quadtree::Quadtree()
{
	m_AreaOne = nullptr;
	m_AreaTwo = nullptr;
	m_AreaThree = nullptr;
	m_AreaFour = nullptr;
}

Quadtree::~Quadtree()
{
}

bool Quadtree::IsContain(Box box)
{
	if (AABB(box, m_Rect) != DIR::NONE)
		return true;

	return false;
}

void Quadtree::insert(vector<GameObject*> list, int level, Box box)
{
	m_Rect = box;
	if (level == 0)
	{
		//m_listObject = list;
		m_listObject.insert(m_listObject.end(), list.begin(), list.end());
	}
	else
	{
		string temp = to_string(level);
		int length = temp.length() - 1;
		int node = level / (int)pow(10.0, length);
		int du = level % (int)pow(10.0, length);

		if (node == 1)
		{
			if (m_AreaOne == nullptr)
				m_AreaOne = new Quadtree();
				
			Box temp(m_Rect.x + m_Rect.width / 2, m_Rect.y + m_Rect.height / 2, m_Rect.width / 2, m_Rect.width / 2);
			m_AreaOne->insert(list, du, temp);
		}
		else if (node == 2)
		{
			if (m_AreaTwo == nullptr)
				m_AreaTwo = new Quadtree();
			
			Box temp(m_Rect.x, m_Rect.y + m_Rect.height / 2, m_Rect.width / 2, m_Rect.width / 2);
			m_AreaTwo->insert(list, du, temp);
		}
		else if (node == 3)
		{
			if (m_AreaThree == nullptr)
				m_AreaThree = new Quadtree();

			Box temp(m_Rect.x, m_Rect.y, m_Rect.width / 2, m_Rect.width / 2);
			m_AreaThree->insert(list, du, temp);
		}
		else if (node == 4)
		{
			if (m_AreaFour == nullptr)
				m_AreaFour = new Quadtree();

			Box temp(m_Rect.x + m_Rect.width / 2, m_Rect.y, m_Rect.width / 2, m_Rect.width / 2);
			m_AreaFour->insert(list, du, temp);
		}	
	}
}

void Quadtree::insert(GameObject* gameObject, int level, Box box)
{
	m_Rect = box;
	if (level == 0)
	{
		m_listObject.push_back(gameObject);
	}
	else
	{
		string temp = to_string(level);
		int length = temp.length() - 1;
		int node = level / (int)pow(10.0, length);
		int du = level % (int)pow(10.0, length);

		if (node == 1)
		{
			if (m_AreaOne == nullptr)
				m_AreaOne = new Quadtree();

			Box temp(m_Rect.x + m_Rect.width / 2, m_Rect.y + m_Rect.height / 2, m_Rect.width / 2, m_Rect.width / 2);
			m_AreaOne->insert(gameObject, du, temp);
		}
		else if (node == 2)
		{
			if (m_AreaTwo == nullptr)
				m_AreaTwo = new Quadtree();

			Box temp(m_Rect.x, m_Rect.y + m_Rect.height / 2, m_Rect.width / 2, m_Rect.width / 2);
			m_AreaTwo->insert(gameObject, du, temp);
		}
		else if (node == 3)
		{
			if (m_AreaThree == nullptr)
				m_AreaThree = new Quadtree();

			Box temp(m_Rect.x, m_Rect.y, m_Rect.width / 2, m_Rect.width / 2);
			m_AreaThree->insert(gameObject, du, temp);
		}
		else if (node == 4)
		{
			if (m_AreaFour == nullptr)
				m_AreaFour = new Quadtree();

			Box temp(m_Rect.x + m_Rect.width / 2, m_Rect.y, m_Rect.width / 2, m_Rect.width / 2);
			m_AreaFour->insert(gameObject, du, temp);
		}
	}
}

void Quadtree::buildTree(map<int, vector<GameObject*>> quadtreeNode, Box box)
{
	for each (pair<int, vector<GameObject*>> var in quadtreeNode)
	{
		if (var.second.size() != 0)
		{
			insert(var.second, var.first, box);
		}
	}
}

vector<GameObject*> Quadtree::getListObjects(Box camera, vector<GameObject*> list)
{
	if ( m_AreaOne != nullptr && m_AreaOne->IsContain(camera))
	{
		vector<GameObject*> temp = m_AreaOne->getListObjects(camera, list);
		list.insert(list.end(), temp.begin(), temp.end());
	}

	if (m_AreaTwo != nullptr && m_AreaTwo->IsContain(camera))
	{
		vector<GameObject*> temp = m_AreaTwo->getListObjects(camera, list);
		list.insert(list.end(), temp.begin(), temp.end());
	}

	if (m_AreaThree != nullptr && m_AreaThree->IsContain(camera))
	{
		vector<GameObject*> temp = m_AreaThree->getListObjects(camera, list);
		list.insert(list.end(), temp.begin(), temp.end());
	}

	if (m_AreaFour != nullptr && m_AreaFour->IsContain(camera))
	{
		vector<GameObject*> temp = m_AreaFour->getListObjects(camera, list);
		list.insert(list.end(), temp.begin(), temp.end());
	}

	// nếu là node lá thì return listObject
	if (m_AreaOne == nullptr && m_AreaTwo == nullptr && m_AreaThree == nullptr && m_AreaFour == nullptr)
	{
		vector<GameObject*> listObjectOnCamera;
		for each (GameObject* var in m_listObject)
		{
			if (AABB(var->getBouding(), camera) != DIR::NONE)
			{
				listObjectOnCamera.push_back(var);
			}
		}

		return listObjectOnCamera;
	}
		

	return list;
}

void Quadtree::remove(GameObject* object)
{
	if (m_AreaOne != nullptr && AABB(m_AreaOne->m_Rect, object->getBouding()) != DIR::NONE)
		m_AreaOne->remove(object);

	if (m_AreaTwo != nullptr && AABB(m_AreaTwo->m_Rect, object->getBouding()) != DIR::NONE)
		m_AreaTwo->remove(object);

	if (m_AreaThree != nullptr && AABB(m_AreaThree->m_Rect, object->getBouding()) != DIR::NONE)
		m_AreaThree->remove(object);

	if (m_AreaFour != nullptr && AABB(m_AreaFour->m_Rect, object->getBouding()) != DIR::NONE)
		m_AreaFour->remove(object);

	if (m_listObject.size() != 0)
	{
		for (vector<GameObject*>::iterator it = m_listObject.begin(); it != m_listObject.end(); it++)
		{
			if (*it == object)
			{
				m_listObject.erase(it);
				break;
			}
		}
	}
}

void Quadtree::update(vector<GameObject*> list)
{
	for each (GameObject* item in list)
	{
		if (item->getTypeObject() == TypeObject::Moving_Enemy)
		{
			this->remove(item);
		}
	}
}