#include "MapObject.h"
#include "SwapAABB.h"
#include "Xml\tinyxml.h"
MapObject::MapObject()
{
	m_Flip = SpriteEffect::None;
	m_Sprite = NULL;
	m_keyboard = NULL;

	m_typeObject = TypeObject::Dynamic_TiledMap;
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
		list = m_quadTree->getListObjects(Box(m_WorldPosition.x, m_WorldPosition.y - VIEW_PORT_Y, SCREEN_WIDTH, SCREEN_HEIGHT), list);
	return list;
}

MapObject::~MapObject()
{
	m_keyboard = NULL;
}