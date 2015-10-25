#include "Map1.h"
#include "ReSource.h"
#include "SwapAABB.h"
#include "LuckyBox.h"

Map1::Map1(Mario *mario)
{
	m_Sprite = ReSource::getInstance()->getSprite(IDImage::IMG_TILEMAP);
	m_WorldPosition.y = VIEW_PORT_Y;

	m_Mario = mario;
}

void Map1::init(CKeyBoard *keyboard)
{
	MapObject::init(keyboard, NodeTileMap_1);

	map<int, vector<GameObject*>> listGameObject;
	for (map<int, vector<ObjectTittle>>::iterator it = m_quadtreeNode.begin(); it != m_quadtreeNode.end(); it++)
	{
		vector<GameObject*> list;
		for (int i = 0; i < it->second.size(); i++)
		{
			if (it->second[i].m_Id == 0) // tile map
			{
				GameObject*	temp = new GameObject();
				temp->setSpriteSheet(m_Sprite);
				temp->setIndexSprite(it->second[i].m_Index);
				temp->setPosition(it->second[i].m_X, it->second[i].m_Y);
				temp->setTypeObject(TypeObject::Dynamic_TiledMap);
				list.push_back(temp);
			}
			else if (it->second[i].m_Id == 1) // stand position
			{
				GameObject*	temp = new GameObject();
				temp->setPosition(it->second[i].m_X, it->second[i].m_Y);
				temp->setBox(Box(it->second[i].m_X, it->second[i].m_Y, it->second[i].m_Width, it->second[i].m_Height));
				temp->setTypeObject(TypeObject::Dynamic_StandPosition);
				list.push_back(temp);
			}
			else if (it->second[i].m_Id == 25) // item box
			{
				GameObject* item = new LuckyBox(ItemsType::IT_GUN);

				item->setPosition(it->second[i].m_X, it->second[i].m_Y); // set position for luckybox
				static_cast<LuckyBox*>(item)->getItem()->setPosition(it->second[i].m_X, it->second[i].m_Y); // set position for item in box
				
				item->setTypeObject(TypeObject::Dynamic_Item);
				list.push_back(item);
			}
		}
		if (list.size() != 0) listGameObject.insert(pair<int, vector<GameObject*>>(it->first, list));
	}

	//build tree
	m_quadTree = new Quadtree();
	m_quadTree->buildTree(listGameObject, Box(0, 0, 3584, 3584));
}


//void Map1::updateCollision()
//{
	//// collision with map
	//// mario
	//if (m_Mario->getFSM() != FSM_Mario::DEAD)
	//{
	//	m_Mario->setDirCollision(DIR::NONE);
	//	m_Mario->setLocation(Location::LOC_IN_AIR);
	//	Box marioBound = GetSweptBroadPhaseBox(m_Mario->getBouding()); // tao vung khong gian cho mario
	//	for each (Box item in getTileNodeOnScreen())
	//	{
	//		DIR dir = m_CheckCollision->isCollision(m_Mario, item);
	//		if (dir != DIR::NONE)
	//		{
	//			if (m_Mario->getFSM() != FSM_Mario::JUMP && dir == DIR::TOP) // fall gặp vật cản
	//				m_Mario->setLocation(Location::LOC_ON_GROUND);
	//		}

	//		if (m_Mario->getDirCollision() == DIR::NONE)
	//			m_Mario->setDirCollision(dir);
	//		
	//	}
	//}
//}

void Map1::update()
{
	//if (m_keyboard->isPressed(DIK_SPACE))
	//	m_Mario->setIsBig(true);
	//else if (m_keyboard->isPressed(DIK_C))
	//	m_Mario->setCanShoot(true);
	//else if (m_keyboard->isPressed(DIK_K))
	//	m_Mario->setDead(true);
	//else if (m_keyboard->isPressed(DIK_V))
	//	m_Mario->setStar(true);
}


void Map1::draw(LPD3DXSPRITE SpriteHandler)
{
	vector<GameObject*> list = m_quadTree->getListObjects(Box(m_WorldPosition.x, m_WorldPosition.y - VIEW_PORT_Y, SCREEN_WIDTH, SCREEN_HEIGHT), list);
	for each (GameObject* var in list)
	{
		if (var->getTypeObject() == TypeObject::Dynamic_TiledMap)
		{
			var->setIndexSprite(var->getIndexSprite());
			var->setWorldPosition(Vector2(m_WorldPosition.x, m_WorldPosition.y));
			var->draw(SpriteHandler);
		}
		else if (var->getTypeObject() != TypeObject::Dynamic_StandPosition)
		{
			var->setWorldPosition(Vector2(m_WorldPosition.x, m_WorldPosition.y));
			var->draw(SpriteHandler);
		}
	}
}

Map1::~Map1()
{
}
