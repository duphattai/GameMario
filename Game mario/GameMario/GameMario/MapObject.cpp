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
#include "Goomba.h"
#include "KoopaTroopa.h"
#include "FireFlower.h"
#include "KoopaTroopaFly.h"
#include "Bowser.h"

MapObject::MapObject()
{
	m_flip = SpriteEffect::None;
	m_sprite = NULL;
	m_typeObject = TypeObject::Dynamic_TiledMap;
	m_sprite = ReSource::getInstance()->getSprite(IDImage::IMG_TILEMAP);

	m_stateMachine = new StateMachine<MapObject>(this);
	m_stateMachine->changeState(MenuGame::getInstance());
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


	// Đọc quadtree từ file xml
	std::map<int, vector<ObjectTittle>> quadtreeNode = readQuadTreeFromFile(FileMap);
	// build tree
	buildQuadTree(quadtreeNode);

	// set color background
	m_colorBackGround = D3DCOLOR_XRGB(146, 144, 255);

	// set time
	ScoreGame::getInstance()->setTimeOfState(400);
}


// < BUILD TREE>
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
	Quadtree::getInstance()->release(); // xóa các node nếu có

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
			{
				list.push_back(temp);
				// Nếu là LuckyBox thì thêm item vào quadtree
				LuckyBox* luckyBox = dynamic_cast<LuckyBox*>(temp);
				if (luckyBox != nullptr)
					list.push_back(luckyBox->getItem());
			}
				
		}
		if (list.size() != 0) listGameObject.insert(pair<int, vector<GameObject*>>(it->first, list));
	}

	// build tree
	// build từ những object không nằm trên biên
	Quadtree::getInstance()->buildTree(listGameObject, Box(0, 0, m_width, m_width));

	// build object nằm trên biên
	for each (ObjectTittle var in listObjectExist)
	{
		GameObject* temp = createGameObject(var);
		if (temp == nullptr)
			continue;

		vector<GameObject*> list;
		list.push_back(createGameObject(var));
		// Nếu là LuckyBox thì thêm item vào quadtree
		LuckyBox* luckyBox = dynamic_cast<LuckyBox*>(temp);
		if (luckyBox != nullptr)
			list.push_back(luckyBox->getItem());
	
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
// </BUILD TREE>


vector<GameObject*> MapObject::getListObjectOnCamera()
{
	vector<GameObject*> list = Quadtree::getInstance()->getListObjects(Camera::getInstance()->getCamera(), list);
	// Các object nằm trên biên chỉ lấy 1 lần trong danh sách
	for (int i = 0; i < list.size() - 1; i++)
	{
		for (int j = i + 1; j < list.size(); j++)
		{
			// Các object cùng địa chỉ
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
	if (!m_stateMachine->isInState(*ChangeMap::getInstance()) && !m_stateMachine->isInState(*MenuGame::getInstance()))
	{
		m_worldPosition = Camera::getInstance()->getViewport();
		vector<GameObject*> list = getListObjectOnCamera();

		// Fire Flower
		for each (GameObject* var in list)
		{
			FireFlower* fireFlower = dynamic_cast<FireFlower*>(var);
			if (fireFlower != nullptr)
			{
				var->setWorldPosition(m_worldPosition);
				var->draw(spriteHandler);
			}
		}
		// Tiled map
		for each (GameObject* var in list)
		{
			if (var->getTypeObject() == TypeObject::Dynamic_TiledMap)
			{
				var->setIndexSprite(var->getIndexSprite());
				var->setWorldPosition(m_worldPosition);
				var->draw(spriteHandler);
			}
		}
		// Item in luckyBox
		for each (GameObject* var in list)
		{
			if (var->getTypeObject() == TypeObject::Moving_Item)
			{
				var->setWorldPosition(m_worldPosition);
				var->draw(spriteHandler);
			}
		}
		// Item, Enemy
		for each (GameObject* var in list)
		{
			if (var->getTypeObject() == TypeObject::Dynamic_Item
				|| var->getTypeObject() == TypeObject::Moving_Enemy)
			{
				if (dynamic_cast<FireFlower*>(var) != nullptr) continue;

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
	// Tile map
	if (gameObject.m_Id == 0) 
	{
		temp = new GameObject();
		temp->setSpriteSheet(ReSource::getInstance()->getSprite(IDImage::IMG_TILEMAP));
		temp->setIndexSprite(gameObject.m_Index);
		temp->setPosition(gameObject.m_X, gameObject.m_Y);
		temp->setTypeObject(TypeObject::Dynamic_TiledMap);
	}

	// Stand position
	else if (gameObject.m_Id == 1) 
	{
		temp = new GameObject();
		temp->setPosition(gameObject.m_X, gameObject.m_Y);
		temp->setBox(Box(gameObject.m_X, gameObject.m_Y, gameObject.m_Width, gameObject.m_Height));
		temp->setTypeObject(TypeObject::Dynamic_StandPosition);
	}

	// 1-Up Mushroom in LuckyBox
	else if (gameObject.m_Id == 20) 
	{
		temp = new LuckyBox(LuckyBoxsType::IT_MUSHROOM_UP, ItemTypes::YellowLuckyBox, m_idMap);

		temp->setPosition(gameObject.m_X, gameObject.m_Y); // set position for luckybox
		static_cast<LuckyBox*>(temp)->getItem()->setPosition(gameObject.m_X, gameObject.m_Y); // set position for item in box

		temp->setTypeObject(TypeObject::Dynamic_Item);
	}

	// Super Mushroom in LuckyBox
	else if (gameObject.m_Id == 21)
	{
		temp = new LuckyBox(LuckyBoxsType::IT_MUSHROOM_BIGGER, ItemTypes::YellowLuckyBox, m_idMap);

		temp->setPosition(gameObject.m_X, gameObject.m_Y); // set position for luckybox
		static_cast<LuckyBox*>(temp)->getItem()->setPosition(gameObject.m_X, gameObject.m_Y); // set position for item in box

		temp->setTypeObject(TypeObject::Dynamic_Item);
	}

	// 1-Up Mushroom Hidden
	else if (gameObject.m_Id == 22) 
	{
		temp = new LuckyBox(LuckyBoxsType::IT_MUSHROOM_UP, ItemTypes::YellowLuckyBox, m_idMap);
		// Thiết lập tọa độ
		temp->setPosition(gameObject.m_X, gameObject.m_Y); // luckybox
		static_cast<LuckyBox*>(temp)->getItem()->setPosition(gameObject.m_X, gameObject.m_Y); // item inside

		temp->setAlphaColor(D3DCOLOR_RGBA(255, 255, 255, 0));
		temp->setTypeObject(TypeObject::Dynamic_Item);
	}

	// Coin
	else if (gameObject.m_Id == 24)
	{
		temp = new Coin(m_idMap);
		// Thiết lập tọa độ
		temp->setPosition(gameObject.m_X, gameObject.m_Y);
		temp->setTypeObject(TypeObject::Dynamic_Item);
	}

	// Luckybox
	else if (gameObject.m_Id == 25) 
	{
		temp = new LuckyBox(LuckyBoxsType::IT_COIN, ItemTypes::YellowLuckyBox, m_idMap);
		// Thiết lập tọa độ
		temp->setPosition(gameObject.m_X, gameObject.m_Y); // Luckybox
		static_cast<LuckyBox*>(temp)->getItem()->setPosition(gameObject.m_X, gameObject.m_Y); // Item inside

		temp->setTypeObject(TypeObject::Dynamic_Item);
	}

	// Floating Bar
	else if (gameObject.m_Id == 26) 
	{
		// Giá trị index tự quy ước
		if (gameObject.m_Index == 1)
			temp = new FloatingBar(FloatingBarMove::MoveUp);
		else if (gameObject.m_Index == 0)
			temp = new FloatingBar(FloatingBarMove::MoveDown);
		// Thiết lập tọa độ
		temp->setPosition(gameObject.m_X, gameObject.m_Y);
		temp->setTypeObject(TypeObject::Dynamic_Item);
	}

	// Brick
	else if (gameObject.m_Id == 27) 
	{
		temp = new Brick(Vector2(gameObject.m_X, gameObject.m_Y), m_idMap);
		temp->setTypeObject(TypeObject::Dynamic_Item);
	}

	// 1-Up Mushroom in Brick
	else if (gameObject.m_Id == 28) 
	{
		temp = new LuckyBox(LuckyBoxsType::IT_MUSHROOM_UP, ItemTypes::BrickLuckyBox, m_idMap);
		// Thiết lập tọa độ
		temp->setPosition(gameObject.m_X, gameObject.m_Y); // luckybox
		static_cast<LuckyBox*>(temp)->getItem()->setPosition(gameObject.m_X, gameObject.m_Y); // Item inside

		temp->setTypeObject(TypeObject::Dynamic_Item);
	}

	// Super Mushroom in Brick
	else if (gameObject.m_Id == 29) 
	{
		temp = new LuckyBox(LuckyBoxsType::IT_MUSHROOM_BIGGER, ItemTypes::BrickLuckyBox, m_idMap);
		// Thiết lập tọa độ
		temp->setPosition(gameObject.m_X, gameObject.m_Y); // luckybox
		static_cast<LuckyBox*>(temp)->getItem()->setPosition(gameObject.m_X, gameObject.m_Y); // item inside

		temp->setTypeObject(TypeObject::Dynamic_Item);
	}

	// Super Star in Brick
	else if (gameObject.m_Id == 30) 
	{
		temp = new LuckyBox(LuckyBoxsType::IT_STAR, ItemTypes::BrickLuckyBox, m_idMap);
		// Thiết lập tọa độ
		temp->setPosition(gameObject.m_X, gameObject.m_Y); // luckybox
		static_cast<LuckyBox*>(temp)->getItem()->setPosition(gameObject.m_X, gameObject.m_Y); // item inside

		temp->setTypeObject(TypeObject::Dynamic_Item);
	}

	// Coin in Brick
	else if (gameObject.m_Id == 32) 
	{
		temp = new LuckyBox(LuckyBoxsType::IT_COIN, ItemTypes::BrickLuckyBox, m_idMap, 4);
		// Thiết lập tọa độ
		temp->setPosition(gameObject.m_X, gameObject.m_Y); // luckybox
		static_cast<LuckyBox*>(temp)->getItem()->setPosition(gameObject.m_X, gameObject.m_Y); // item inside

		temp->setTypeObject(TypeObject::Dynamic_Item);
	}

	// Item Flag
	else if (gameObject.m_Id == 33)
	{
		temp = new Flag(Vector2(gameObject.m_X, gameObject.m_Y));
		temp->setTypeObject(TypeObject::Dynamic_Item);
	}

	else if (gameObject.m_Id == 10) // Goomba
	{
		temp = new Goomba();
		temp->setPosition(gameObject.m_X, gameObject.m_Y); // set position for MushroomEnemy
	}
	else if (gameObject.m_Id == 14) // KoopaTrooppa
	{
		temp = new KoopaTroopa();
		temp->setPosition(gameObject.m_X, gameObject.m_Y); // set position for MushroomEnemy
	}

	else if (gameObject.m_Id == 12) // FlowerEnemy
	{
		temp = new FireFlower();
		temp->setPosition(gameObject.m_X + 8, gameObject.m_Y); // set position for FlowerEnemy
	}

	else if (gameObject.m_Id == 17) // KoopaTrooppaFly
	{
		temp = new KoopaTroopaFly();
		temp->setPosition(gameObject.m_X, gameObject.m_Y); // set position for FlowerEnemy
	}

	else if (gameObject.m_Id == 16) // Boss
	{
		temp = new Bowser();
		temp->setPosition(gameObject.m_X, gameObject.m_Y); // set position for FlowerEnemy
	}


	return temp;
}


void MapObject::update()
{
	if (m_stateMachine->isInState(*MenuGame::getInstance())  // Màn hình giới thiệu
		|| m_stateMachine->isInState(*ChangeMap::getInstance())) // Chuyển map
		return;

	// Danh sách object trên camera
	vector<GameObject*> listObjectOnCamera = getListObjectOnCamera();


    // < CẬP NHẬT SUPER MUSHROOM, FLOWER GUN. NẾU SUPER, FIRE MARIO THÌ SUPER MUSHROOM -> FLOWER GUN >
	for each (GameObject* var in listObjectOnCamera)
	{
		// là dynamic item
		if (var->getTypeObject() == TypeObject::Dynamic_Item)
		{
			LuckyBox* item = dynamic_cast<LuckyBox*>(var);
			if (item != nullptr && !item->getItem()->isActive()) // cập nhật đối với item còn trong luckybox
			{
				// Super mushroom
				if (item->getItem()->getItemType() == LuckyBoxsType::IT_MUSHROOM_BIGGER)
				{
					// trạng thái Super mario hoặc Fire mario
					if (Mario::getInstance()->isBig() || Mario::getInstance()->canShoot())
						item->getItem()->setItemType(LuckyBoxsType::IT_GUN);
				}
				// Flower gun
				else if (item->getItem()->getItemType() == LuckyBoxsType::IT_GUN)
				{
					// trạng thái Small mario
					if (!Mario::getInstance()->isBig() && !Mario::getInstance()->canShoot())
						item->getItem()->setItemType(LuckyBoxsType::IT_MUSHROOM_BIGGER);
				}
			}
		}
	}
	// </>

	// < XÉT VA CHẠM >
	for each (GameObject* item in listObjectOnCamera)
	{
		// Item, Enemy
		int type = item->getTypeObject();
		if (type == TypeObject::Moving_Enemy || type == TypeObject::Moving_Item 
			|| type == TypeObject::Dynamic_Item)
		{
			for each (GameObject* temp in listObjectOnCamera)
			{
				if (temp != item) // Không xét va chạm với chính nó
					item->isCollision(temp);
			}
				

			// Với mario
			item->isCollision(Mario::getInstance());
		}
		// Mario
		else if (type == TypeObject::Dynamic_StandPosition)
			Mario::getInstance()->isCollision(item);

		// Đạn
		Mario::getInstance()->getGun()->isCollision(item);
	}
	// </>


	// < CẬP NHẬT TỌA ĐỘ >
	// Item, Enemy
	for each (GameObject* item in listObjectOnCamera)
	{
		if (item->getTypeObject() == TypeObject::Dynamic_Item || item->getTypeObject() == TypeObject::Moving_Item
			|| item->getTypeObject() == TypeObject::Moving_Enemy)
			item->update();
	}
	
	// Mario
	Mario::getInstance()->update();
	
	// Súng
	Mario::getInstance()->getGun()->update();
	// </>


	// CẬP NHẬT VIEWPORT
	if (m_isScrollMap)
		Camera::getInstance()->update(Mario::getInstance()->getPosition());
	

	// CẬP NHẬT QUADTREE
	Quadtree::getInstance()->update(listObjectOnCamera, Camera::getInstance()->getCamera());

	// CẬP NHẬT SCOREGAME
	ScoreGame::getInstance()->update();
}


void MapObject::updateVelocity()
{
	m_stateMachine->update();
	if (m_stateMachine->isInState(*MenuGame::getInstance()) // Màn hình giới thiệu 
		|| m_stateMachine->isInState(*ChangeMap::getInstance())) // Chuyển map
		return;


	// < CẬP NHẬT VẬN TỐC>
	// Đạn
	Mario::getInstance()->getGun()->updateVelocity();
	// Mario
	Mario::getInstance()->updateVelocity();
	// Item, Enemy
	for each (GameObject* item in getListObjectOnCamera())
	{
		if (item->getTypeObject() == TypeObject::Dynamic_Item || item->getTypeObject() == TypeObject::Moving_Item 
			|| item->getTypeObject() == TypeObject::Moving_Enemy)
			item->updateVelocity();
	}
	// </>
 }