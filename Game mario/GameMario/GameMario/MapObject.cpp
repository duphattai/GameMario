#include "MapObject.h"
#include "SwapAABB.h"
#include "Xml\tinyxml.h"
#include "LuckyBox.h"
#include "Brick.h"

MapObject::MapObject()
{
	m_flip = SpriteEffect::None;
	m_sprite = NULL;
	m_keyboard = NULL;

	m_typeObject = TypeObject::Dynamic_TiledMap;
}

MapObject::~MapObject()
{
	m_keyboard = NULL;
}

map<int, vector<ObjectTittle>> readQuadTreeFromFile(TiXmlElement *nodeParent)
{
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

void MapObject::init(CKeyBoard *keyboard, char *FileMap)
{
	// 16/11
	// read file map xml
	TiXmlDocument doc(FileMap);
	if (!doc.LoadFile())
	{
		printf("%s", doc.ErrorDesc());
		return;
	}

	TiXmlElement* root = doc.RootElement();
	TiXmlElement* node = root->FirstChildElement(); // node -> information

	TiXmlElement* quadTreeNode = node->NextSiblingElement(); // node ->Quadtree
	m_quadtreeNode = readQuadTreeFromFile(quadTreeNode);
	//end

	m_keyboard = keyboard;
	m_colorBackGround = D3DCOLOR_XRGB(146, 144, 255);
}

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
	else if (gameObject.m_Id == 25) // item box
	{
		temp = new LuckyBox(ItemsType::IT_COIN);

		temp->setPosition(gameObject.m_X, gameObject.m_Y); // set position for luckybox
		static_cast<LuckyBox*>(temp)->getItem()->setPosition(gameObject.m_X, gameObject.m_Y); // set position for item in box

		temp->setTypeObject(TypeObject::Dynamic_Item);
	}
	else if (gameObject.m_Id == 21) // item mushroom bigger
	{
		temp = new LuckyBox(ItemsType::IT_MUSHROOM_BIGGER);

		temp->setPosition(gameObject.m_X, gameObject.m_Y); // set position for luckybox
		static_cast<LuckyBox*>(temp)->getItem()->setPosition(gameObject.m_X, gameObject.m_Y); // set position for item in box

		temp->setTypeObject(TypeObject::Dynamic_Item);
	}
	else if (gameObject.m_Id == 27) // item brick
	{
		Vector2 position(gameObject.m_X, gameObject.m_Y);
		temp = new Brick(position);
		temp->setTypeObject(TypeObject::Dynamic_Item);
	}

	return temp;
}