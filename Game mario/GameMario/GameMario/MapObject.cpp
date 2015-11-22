#include "MapObject.h"
#include "SwapAABB.h"
#include "LuckyBox.h"
#include "Brick.h"
#include "Coin.h"


MapObject::MapObject()
{
	m_flip = SpriteEffect::None;
	m_sprite = NULL;

	m_typeObject = TypeObject::Dynamic_TiledMap;
}

MapObject::~MapObject()
{
}

map<int, vector<ObjectTittle>> MapObject::readQuadTreeFromFile(TiXmlElement *nodeParent)
{
	nodeParent = nodeParent->FirstChildElement()->NextSiblingElement(); // trỏ tới node có tên quadtree
	TiXmlElement* node = nodeParent->FirstChildElement(); // get first node
	map<int, vector<ObjectTittle>> list;

	while (node != nullptr)
	{
		int id;
		node->QueryIntAttribute("id", &id);

		vector<ObjectTittle> listObject;
		TiXmlElement* object = node->FirstChildElement(); // get first object in node
		while (object != nullptr) // get all objects in child node
		{
			ObjectTittle temp;
			object->QueryIntAttribute("id", &temp.m_Id);
			object->QueryIntAttribute("index", &temp.m_Index);
			object->QueryIntAttribute("x", &temp.m_X);
			object->QueryIntAttribute("y", &temp.m_Y);
			object->QueryIntAttribute("width", &temp.m_Width);
			object->QueryIntAttribute("height", &temp.m_Height);

			listObject.push_back(temp);
			object = object->NextSiblingElement(); // get next object
		}

		// insert list object
		list.insert(std::pair<int, vector<ObjectTittle>>(id, listObject));
		node = node->NextSiblingElement();//get next node
	}

	return list;
}

void MapObject::init(char *FileMap)
{
	// 16/11
	// đọc quadtree từ file xml
	TiXmlDocument doc(FileMap);
	if (!doc.LoadFile())
	{
		printf("%s", doc.ErrorDesc());
		return;
	}
	m_quadtreeNode = readQuadTreeFromFile(doc.RootElement());
	// end


	// build tree
	buildQuadTree();
	m_colorBackGround = D3DCOLOR_XRGB(146, 144, 255);
}

// <build tree>

// kiểm tra node có nằm ở trên biên
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

void MapObject::buildQuadTree()
{
	vector<ObjectTittle> listObjectExist; // chứa danh sách object nằm trên biên
	map<int, vector<GameObject*>> listGameObject; // chưa danh sách object trong cây (không nằm trên biên)

	for (map<int, vector<ObjectTittle>>::iterator it = m_quadtreeNode.begin(); it != m_quadtreeNode.end(); it++)
	{
		vector<GameObject*> list; // danh sách object không nằm trên biên
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

				// quản lý đối tượng nằm trên biên
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
	// build từ những object không nằm trên biên
	m_quadTree->buildTree(listGameObject, Box(0, 0, 3584, 3584));

	// build object nằm trên biên
	for each (ObjectTittle var in listObjectExist)
	{
		vector<GameObject*> list;
		GameObject* temp = createGameObject(var);
		if (temp == nullptr)
			continue;

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
// </build tree>


vector<GameObject*> MapObject::getListObjectOnCamera()
{
	vector<GameObject*> list;
	if (m_quadTree != nullptr)
	{
		list = m_quadTree->getListObjects(Box(m_worldPosition.x, m_worldPosition.y - VIEW_PORT_Y, SCREEN_WIDTH, SCREEN_HEIGHT), list);

		// các object nằm trên biên chỉ lấy 1 lần trong danh sách
		for (int i = 0; i < list.size() - 1; i++)
		{
			for (int j = i + 1; j < list.size(); j++)
			{
				if (list[i] == list[j])
				{
					list.erase(list.begin() + j);
					break;
				}
			}
		}
	}
		

	return list;
}

void MapObject::draw(LPD3DXSPRITE SpriteHandler)
{
	vector<GameObject*> list = getListObjectOnCamera();
	if (list.size() == 0) return;


	// vẽ tiled map trước
	for each (GameObject* var in list)
	{
		if (var->getTypeObject() == TypeObject::Dynamic_TiledMap)
		{
			var->setIndexSprite(var->getIndexSprite());
			var->setWorldPosition(Vector2(m_worldPosition.x, m_worldPosition.y));
			var->draw(SpriteHandler);
		}
	}
	// vẽ item và enemy sau
	for each (GameObject* var in list)
	{
		if (var->getTypeObject() == TypeObject::Dynamic_Item || var->getTypeObject() == TypeObject::Moving_Enemy)
		{
			var->setWorldPosition(Vector2(m_worldPosition.x, m_worldPosition.y));
			var->draw(SpriteHandler);
		}
	}
}

GameObject* MapObject::createGameObject(ObjectTittle gameObject)
{
	GameObject*	temp = nullptr;
	if (gameObject.m_Id == 0) // tile map
	{
		temp = new GameObject();
		temp->setSpriteSheet(m_sprite);
		temp->setIndexSprite(gameObject.m_Index);
		temp->setPosition(gameObject.m_X, gameObject.m_Y);
		temp->setTypeObject(TypeObject::Dynamic_TiledMap);
	}
	else if (gameObject.m_Id == 1) // stand position
	{
		temp = new GameObject();
		temp->setPosition(gameObject.m_X, gameObject.m_Y);
		temp->setBox(Box(gameObject.m_X, gameObject.m_Y, gameObject.m_Width, gameObject.m_Height));
		temp->setTypeObject(TypeObject::Dynamic_StandPosition);
	}
	else if (gameObject.m_Id == 25) // coin in luckybox
	{
		temp = new LuckyBox(LuckyBoxsType::IT_COIN, ItemTypes::YellowLuckyBox);

		temp->setPosition(gameObject.m_X, gameObject.m_Y); // set position for luckybox
		static_cast<LuckyBox*>(temp)->getItem()->setPosition(gameObject.m_X, gameObject.m_Y); // set position for item in box

		temp->setTypeObject(TypeObject::Dynamic_Item);
	}
	else if (gameObject.m_Id == 21) // item mushroom bigger
	{
		temp = new LuckyBox(LuckyBoxsType::IT_MUSHROOM_BIGGER, ItemTypes::YellowLuckyBox);

		temp->setPosition(gameObject.m_X, gameObject.m_Y); // set position for luckybox
		static_cast<LuckyBox*>(temp)->getItem()->setPosition(gameObject.m_X, gameObject.m_Y); // set position for item in box

		temp->setTypeObject(TypeObject::Dynamic_Item);
	}
	else if (gameObject.m_Id == 27) // brick
	{
		Vector2 position(gameObject.m_X, gameObject.m_Y);
		temp = new Brick(position);
		temp->setTypeObject(TypeObject::Dynamic_Item);
	}
	else if (gameObject.m_Id == 32) // coin in brick
	{
		temp = new LuckyBox(LuckyBoxsType::IT_COIN, ItemTypes::BrickLuckyBox, 4);

		temp->setPosition(gameObject.m_X, gameObject.m_Y); // set position for luckybox
		static_cast<LuckyBox*>(temp)->getItem()->setPosition(gameObject.m_X, gameObject.m_Y); // set position for item in box

		temp->setTypeObject(TypeObject::Dynamic_Item);
	}
	else if (gameObject.m_Id == 30) // star in brick
	{
		temp = new LuckyBox(LuckyBoxsType::IT_STAR, ItemTypes::BrickLuckyBox);

		temp->setPosition(gameObject.m_X, gameObject.m_Y); // set position for luckybox
		static_cast<LuckyBox*>(temp)->getItem()->setPosition(gameObject.m_X, gameObject.m_Y); // set position for item in box

		temp->setTypeObject(TypeObject::Dynamic_Item);
	}
	return temp;
}

void MapObject::update(Mario* mario)
{
	vector<GameObject*> list = getListObjectOnCamera();
	// cập nhật loại item bigger, nếu mario trong trạng thái to thì bigger -> gun
	for each (GameObject* var in list)
	{
		if (var->getTypeObject() == TypeObject::Dynamic_Item)
		{
			LuckyBox* luckyBox = dynamic_cast<LuckyBox*>(var);
			if (luckyBox != nullptr && !luckyBox->getItem()->isActive())
			{
				if (luckyBox->getType() == LuckyBoxsType::IT_MUSHROOM_BIGGER)
				{
					if (mario->isBig() || mario->canShoot())
						luckyBox->changeItemsType(LuckyBoxsType::IT_GUN);
				}
				else if (luckyBox->getType() == LuckyBoxsType::IT_GUN)
				{
					if (!mario->isBig() && !mario->canShoot())
						luckyBox->changeItemsType(LuckyBoxsType::IT_MUSHROOM_BIGGER);
				}
			}
		}
	}

	// cập nhật quadtree
	m_quadTree->update(list, mario->getCamera());
}