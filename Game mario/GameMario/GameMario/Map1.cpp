#include "Map1.h"
#include "ReSource.h"
#include "SwapAABB.h"
#include "LuckyBox.h"
#include "Brick.h"

Map1::Map1(Mario *mario)
{
	m_sprite = ReSource::getInstance()->getSprite(IDImage::IMG_TILEMAP);
	m_worldPosition.y = VIEW_PORT_Y;

	m_Mario = mario;
}

// kiểm tra node có nằm ở trên biên trong quadtree
bool checkExist(ObjectTittle temp, map<int, vector<ObjectTittle>> &treeNode)
{
	int count = 0; // xác định số lần trùng
	for (map<int, vector<ObjectTittle>>::iterator it = treeNode.begin(); it != treeNode.end(); it++)
	{
		for (int i = 0; i < it->second.size(); i++)
		{
			if (temp == it->second[i])
			{
				count++;
				break;
			}
		}
	}

	if (count > 1) return true; // nếu count > 1 nghĩa là tồn tại node nằm trên biên
	return false;
}


void Map1::init(CKeyBoard *keyboard)
{
	MapObject::init(keyboard, NodeTileMap_1);

	vector<ObjectTittle> listObjectExist; // chứa danh sách object nằm trên biên
	map<int, vector<GameObject*>> listGameObject; // chưa danh sách object trong cây (không nằm trên biên)
	for (map<int, vector<ObjectTittle>>::iterator it = m_quadtreeNode.begin(); it != m_quadtreeNode.end(); it++)
	{
		vector<GameObject*> list;
		for (int i = 0; i < it->second.size(); i++)
		{
			if (checkExist(it->second[i], m_quadtreeNode)) // nằm trên biên
			{
				// kiểm tra object muốn thêm vào đã tồn tại trong listObjectExist hay chưa
				bool exist = false;
				for each (ObjectTittle var in listObjectExist)
				{
					if (var == it->second[i])
					{
						exist = true;
						break;
					}
				}

				if (!exist)
					listObjectExist.push_back(it->second[i]);
				continue;
			}

			// chỉ tạo object nào không nằm trên biên
			GameObject*	temp = createGameObject(it->second[i]);
			if (temp != nullptr)
				list.push_back(temp);
		}
		if (list.size() != 0) listGameObject.insert(pair<int, vector<GameObject*>>(it->first, list));
	}

	//build tree
	m_quadTree = new Quadtree();
	m_quadTree->buildTree(listGameObject, Box(0, 0, 3584, 3584)); // build từ những object không nằm trên biên
	for each (ObjectTittle var in listObjectExist)// build object nằm trên biên
	{
		vector<GameObject*> list;
		list.push_back(createGameObject(var));
		for (map<int, vector<ObjectTittle>>::iterator it = m_quadtreeNode.begin(); it != m_quadtreeNode.end(); it++)
		{
			for (int i = 0; i < it->second.size(); i++)
			{
				if (var == it->second[i]) // nằm trên biên
				{
					m_quadTree->insert(list, it->first, Box(0, 0, 3584, 3584));
					break;
				}
			}
		}
	}
}

void Map1::update()
{
}


Map1::~Map1()
{
}
