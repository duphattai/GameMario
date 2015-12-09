#include "MapObject.h"
#include "SwapAABB.h"
#include "LuckyBox.h"
#include "Brick.h"
#include "Coin.h"
#include "MapOwnedState.h"
#include "ReSource.h"
#include "Camera.h"
#include "ScoreGame.h"
#include "MarioOwnedState.h"
#include "Flag.h"
#include "FloatingBar.h"



MapObject::MapObject()
{
	m_flip = SpriteEffect::None;
	m_sprite = NULL;
	m_typeObject = TypeObject::Dynamic_TiledMap;
	m_sprite = ReSource::getInstance()->getSprite(IDImage::IMG_TILEMAP);

	m_stateMachine = new StateMachine<MapObject>(this);
	m_stateMachine->changeState(BrosTitle::getInstance());
}

MapObject::~MapObject()
{
	delete m_stateMachine;
}

map<int, vector<ObjectTittle>> MapObject::readQuadTreeFromFile(char* path)
{
	TiXmlDocument doc(path);
	if (!doc.LoadFile())
	{
		printf("%s", doc.ErrorDesc());
		exit(0);
	}
	TiXmlElement* node = doc.RootElement()->FirstChildElement()->NextSiblingElement(); // get first node
	node->QueryIntAttribute("mapwidth", &m_width);
	m_height = 240;

	node = node->FirstChildElement();
	map<int, vector<ObjectTittle>> list;
	while (node != nullptr)
	{
		int id;
		node->QueryIntAttribute("id", &id);

		vector<ObjectTittle> listObject;
		TiXmlElement* object = node->FirstChildElement(); // lấy object con của node
		while (object != nullptr) // tồn tại object con
		{
			ObjectTittle temp;
			object->QueryIntAttribute("id", &temp.m_Id);
			object->QueryIntAttribute("index", &temp.m_Index);
			object->QueryIntAttribute("x", &temp.m_X);
			object->QueryIntAttribute("y", &temp.m_Y);
			object->QueryIntAttribute("width", &temp.m_Width);
			object->QueryIntAttribute("height", &temp.m_Height);

			listObject.push_back(temp);
			object = object->NextSiblingElement(); // lấy object con kế tiếp
		}

		// insert list object
		list.insert(std::pair<int, vector<ObjectTittle>>(id, listObject));
		node = node->NextSiblingElement();//get next node
	}

	return list;
}

void MapObject::init(IDMap map)
{
	char *FileMap = nullptr;
	m_idMap = map;
	// thiết lập đường dẫn
	if (map == IDMap::MapOne)
	{
		FileMap = NodeTileMap_1;
		m_checkPoint = Vector2(1280, 32);
	}
	else if (map == IDMap::MapTwo)
	{
		FileMap = "Map//1-2//TileNode.xml";
		m_checkPoint = Vector2(1872, 32);
	}
	else if (map == IDMap::MapThree)
	{
		FileMap = "Map//1-3//TileNode.xml";
		m_checkPoint = Vector2(1056, 32);
	}


	// đọc quadtree từ file xml
	std::map<int, vector<ObjectTittle>> quadtreeNode = readQuadTreeFromFile(FileMap);
	// build tree
	buildQuadTree(quadtreeNode);

	// set color background
	m_colorBackGround = D3DCOLOR_XRGB(146, 144, 255);

	// set time
	ScoreGame::getInstance()->setTimeOfState(400);
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

void MapObject::buildQuadTree(map<int, vector<ObjectTittle>>	quadtreeNode)
{
	vector<ObjectTittle> listObjectExist; // chứa danh sách object nằm trên biên
	map<int, vector<GameObject*>> listGameObject; // chưa danh sách object trong cây (không nằm trên biên)

	for (map<int, vector<ObjectTittle>>::iterator it = quadtreeNode.begin(); it != quadtreeNode.end(); it++)
	{
		vector<GameObject*> list; // danh sách object không nằm trên biên
		for (int i = 0; i < it->second.size(); i++)
		{
			if (checkExist(it->second[i], quadtreeNode)) // nằm trên biên
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

	// build tree
	// build từ những object không nằm trên biên
 	Quadtree::getInstance()->release(); // xóa các node nếu có
	Quadtree::getInstance()->buildTree(listGameObject, Box(0, 0, m_width, m_width));


	// build object nằm trên biên
	for each (ObjectTittle var in listObjectExist)
	{
		vector<GameObject*> list;
		GameObject* temp = createGameObject(var);
		if (temp == nullptr)
			continue;

		list.push_back(createGameObject(var));
		for (map<int, vector<ObjectTittle>>::iterator it = quadtreeNode.begin(); it != quadtreeNode.end(); it++)
		{
			for (int i = 0; i < it->second.size(); i++)
			{
				if (var == it->second[i]) // nằm trên biên
				{
					Quadtree::getInstance()->insert(list, it->first, Box(0, 0, m_width, m_width));
					break;
				}
			}
		}
	}
}
// </build tree>

vector<GameObject*> MapObject::getListObjectOnCamera()
{
	vector<GameObject*> list = Quadtree::getInstance()->getListObjects(Camera::getInstance()->getCamera(), list);
	// các object nằm trên biên chỉ lấy 1 lần trong danh sách
	for (int i = 0; i < list.size() - 1; i++)
	{
		for (int j = i + 1; j < list.size(); j++)
		{
			if (list[i] == list[j])
			{
				list.erase(list.begin() + j);
				j--;
			}
		}
	}
	
		
	return list;
}

void MapObject::draw(LPD3DXSPRITE spriteHandler)
{
	if (!m_stateMachine->isInState(*ChangeMap::getInstance()) && !m_stateMachine->isInState(*BrosTitle::getInstance()))
	{
		m_worldPosition = Camera::getInstance()->getViewport();
		vector<GameObject*> list = getListObjectOnCamera();

		// vẽ tiled map trước
		for each (GameObject* var in list)
		{
			if (var->getTypeObject() == TypeObject::Dynamic_TiledMap)
			{
				var->setIndexSprite(var->getIndexSprite());
				var->setWorldPosition(m_worldPosition);
				var->draw(spriteHandler);
			}
		}
		// vẽ item và enemy sau
		for each (GameObject* var in list)
		{
			if (var->getTypeObject() == TypeObject::Dynamic_Item || var->getTypeObject() == TypeObject::Moving_Enemy)
			{
				var->setWorldPosition(m_worldPosition);
				var->draw(spriteHandler);
			}
		}
	}

	m_stateMachine->GetCurrentState()->draw(this, spriteHandler);
}

GameObject* MapObject::createGameObject(ObjectTittle gameObject)
{
	GameObject*	temp = nullptr;
	if (gameObject.m_Id == 0) // tile map
	{
		temp = new GameObject();
		temp->setSpriteSheet(ReSource::getInstance()->getSprite(IDImage::IMG_TILEMAP));
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
	else if (gameObject.m_Id == 20) // 1-Up Mushroom in LuckyBox
	{
		temp = new LuckyBox(LuckyBoxsType::IT_MUSHROOM_UP, ItemTypes::YellowLuckyBox, m_idMap);

		temp->setPosition(gameObject.m_X, gameObject.m_Y); // set position for luckybox
		static_cast<LuckyBox*>(temp)->getItem()->setPosition(gameObject.m_X, gameObject.m_Y); // set position for item in box

		temp->setTypeObject(TypeObject::Dynamic_Item);
	}
	else if (gameObject.m_Id == 21) // Super Mushroom in LuckyBox
	{
		temp = new LuckyBox(LuckyBoxsType::IT_MUSHROOM_BIGGER, ItemTypes::YellowLuckyBox, m_idMap);

		temp->setPosition(gameObject.m_X, gameObject.m_Y); // set position for luckybox
		static_cast<LuckyBox*>(temp)->getItem()->setPosition(gameObject.m_X, gameObject.m_Y); // set position for item in box

		temp->setTypeObject(TypeObject::Dynamic_Item);
	}
	else if (gameObject.m_Id == 22) // 1-Up Mushroom Hidden
	{
		temp = new LuckyBox(LuckyBoxsType::IT_MUSHROOM_UP, ItemTypes::YellowLuckyBox, m_idMap);

		temp->setPosition(gameObject.m_X, gameObject.m_Y); // set position for luckybox
		static_cast<LuckyBox*>(temp)->getItem()->setPosition(gameObject.m_X, gameObject.m_Y); // set position for item in box

		temp->setAlphaColor(D3DCOLOR_RGBA(255, 255, 255, 0));
		temp->setTypeObject(TypeObject::Dynamic_Item);
	}
	else if (gameObject.m_Id == 24) // Coin
	{
		temp = new Coin(m_idMap);
		temp->setPosition(gameObject.m_X, gameObject.m_Y); // set position for coin
		temp->setTypeObject(TypeObject::Dynamic_Item);
	}
	else if (gameObject.m_Id == 25) // Luckybox
	{
		temp = new LuckyBox(LuckyBoxsType::IT_COIN, ItemTypes::YellowLuckyBox, m_idMap);

		temp->setPosition(gameObject.m_X, gameObject.m_Y); // set position for luckybox
		static_cast<LuckyBox*>(temp)->getItem()->setPosition(gameObject.m_X, gameObject.m_Y); // set position for item in box

		temp->setTypeObject(TypeObject::Dynamic_Item);
	}
	else if (gameObject.m_Id == 26) // Floating Bar
	{
		// các giá trị index tự quy ước
		if (gameObject.m_Index == 1)
			temp = new FloatingBar(FloatingBarMove::MoveUp);
		else if (gameObject.m_Index == 0)
			temp = new FloatingBar(FloatingBarMove::MoveDown);
		temp->setPosition(gameObject.m_X, gameObject.m_Y); // set position for luckybox
		temp->setTypeObject(TypeObject::Dynamic_Item);
	}
	else if (gameObject.m_Id == 27) // Brick
	{
		Vector2 position(gameObject.m_X, gameObject.m_Y);
		temp = new Brick(position, m_idMap);
		temp->setTypeObject(TypeObject::Dynamic_Item);
	}
	else if (gameObject.m_Id == 28) // 1-Up Mushroom in Brick
	{
		temp = new LuckyBox(LuckyBoxsType::IT_MUSHROOM_UP, ItemTypes::BrickLuckyBox, m_idMap);

		temp->setPosition(gameObject.m_X, gameObject.m_Y); // set position for luckybox
		static_cast<LuckyBox*>(temp)->getItem()->setPosition(gameObject.m_X, gameObject.m_Y); // set position for item in box

		temp->setTypeObject(TypeObject::Dynamic_Item);
	}
	else if (gameObject.m_Id == 29) // Super Mushroom in Brick
	{
		temp = new LuckyBox(LuckyBoxsType::IT_MUSHROOM_BIGGER, ItemTypes::BrickLuckyBox, m_idMap);

		temp->setPosition(gameObject.m_X, gameObject.m_Y); // set position for luckybox
		static_cast<LuckyBox*>(temp)->getItem()->setPosition(gameObject.m_X, gameObject.m_Y); // set position for item in box

		temp->setTypeObject(TypeObject::Dynamic_Item);
	}
	else if (gameObject.m_Id == 30) // Super Star in Brick
	{
		temp = new LuckyBox(LuckyBoxsType::IT_STAR, ItemTypes::BrickLuckyBox, m_idMap);

		temp->setPosition(gameObject.m_X, gameObject.m_Y); // set position for luckybox
		static_cast<LuckyBox*>(temp)->getItem()->setPosition(gameObject.m_X, gameObject.m_Y); // set position for item in box

		temp->setTypeObject(TypeObject::Dynamic_Item);
	}
	else if (gameObject.m_Id == 32) // Coin in Brick
	{
		temp = new LuckyBox(LuckyBoxsType::IT_COIN, ItemTypes::BrickLuckyBox, m_idMap, 4);

		temp->setPosition(gameObject.m_X, gameObject.m_Y); // set position for luckybox
		static_cast<LuckyBox*>(temp)->getItem()->setPosition(gameObject.m_X, gameObject.m_Y); // set position for item in box

		temp->setTypeObject(TypeObject::Dynamic_Item);
	}
	else if (gameObject.m_Id == 33) // item Flag
	{
		temp = new Flag(Vector2(gameObject.m_X, gameObject.m_Y));
		temp->setTypeObject(TypeObject::Dynamic_Item);
	}
	
	return temp;
}

void MapObject::update()
{
	// nếu đang ở state bros title thì không làm gì thêm
	if (m_stateMachine->isInState(*BrosTitle::getInstance()) || m_stateMachine->isInState(*ChangeMap::getInstance()))
		return;

	// <cập nhật loại item bigger, nếu mario trong trạng thái to thì bigger -> gun>
	vector<GameObject*> listObjectOnCamera = getListObjectOnCamera();
	for each (GameObject* var in listObjectOnCamera)
	{
		if (var->getTypeObject() == TypeObject::Dynamic_Item)
		{
			LuckyBox* luckyBox = dynamic_cast<LuckyBox*>(var);
			if (luckyBox != nullptr && !luckyBox->getItem()->isActive())
			{
				if (luckyBox->getTypeItem() == LuckyBoxsType::IT_MUSHROOM_BIGGER)
				{
					if (Mario::getInstance()->isBig() || Mario::getInstance()->canShoot())
						luckyBox->changeItemsType(LuckyBoxsType::IT_GUN);
				}
				else if (luckyBox->getTypeItem() == LuckyBoxsType::IT_GUN)
				{
					if (!Mario::getInstance()->isBig() && !Mario::getInstance()->canShoot())
						luckyBox->changeItemsType(LuckyBoxsType::IT_MUSHROOM_BIGGER);
				}
			}
		}
	}
	// </>

	// <xét va chạm>
	for each (GameObject* item in listObjectOnCamera)
	{
		// xét va chạm cho item, enemy với stand position
		int type = item->getTypeObject();
		if (type == TypeObject::Moving_Enemy || type == TypeObject::Dynamic_Item)
		{
			for each (GameObject* temp in listObjectOnCamera)
				item->isCollision(temp);
		}

		// xét va chạm mario với object trong game
		Mario::getInstance()->isCollision(item);

		// xét va chạm cho đạn
		Mario::getInstance()->getGun()->isCollision(item);
	}
	//</>


	// <cập nhật tọa độ>
	for each (GameObject* item in listObjectOnCamera)
	{
		if (item->getTypeObject() == TypeObject::Dynamic_Item 
			|| item->getTypeObject() == TypeObject::Moving_Enemy)
			item->update();
	}
	Mario::getInstance()->update(); // cập nhật cho mario
	// <Gun>
	// hủy đạn khi ra khỏi màn hình
	for each (Bullet* bullet in Mario::getInstance()->getGun()->getBulletShooted())
	{
		if (bullet->isActive() && bullet->getPosition().x < Camera::getInstance()->getViewport().x
			|| bullet->getPosition().x > Camera::getInstance()->getViewport().x + SCREEN_WIDTH)
			bullet->setTimeToLive(0);
	}
	// cập nhật cho súng
	Mario::getInstance()->getGun()->update();
	// </>


	// chỉ cập nhật viewport khi mario khong trong trạng thái auto animation
	if (m_isScrollMap)
		Camera::getInstance()->update(Mario::getInstance()->getPosition()); // cập nhật lại viewport
	

	// cập nhật quadtree
	Quadtree::getInstance()->update(listObjectOnCamera, Camera::getInstance()->getCamera());

	// cập nhật scoreGame
	ScoreGame::getInstance()->update();
}

void MapObject::updateVelocity()
{
	m_stateMachine->update();
	// nếu đang ở state bros title thì không làm gì thêm
	if (m_stateMachine->isInState(*BrosTitle::getInstance()) || m_stateMachine->isInState(*ChangeMap::getInstance()))
		return;

	// <cập nhật vận tốc>
	Mario::getInstance()->getGun()->updateVelocity(); // đạn
	Mario::getInstance()->updateVelocity(); // mario
	// cập nhật cho object game
	for each (GameObject* item in getListObjectOnCamera())
	{
		if (item->getTypeObject() == TypeObject::Dynamic_Item || item->getTypeObject() == TypeObject::Moving_Enemy)
			item->updateVelocity();
	}
	// </>
}