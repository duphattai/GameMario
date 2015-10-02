#include "MapObject.h"
#include "SwapAABB.h"
MapObject::MapObject()
{
	m_Flip = SpriteEffect::None;
	m_Sprite = NULL;
	m_keyboard = NULL;

}

void MapObject::init(CKeyBoard *keyboard, char *FileMap)
{
	ifstream file;
	file.open(FileMap);

	char s[1000];
	for (int i = 0; i < Line; i++)
	{
		file.getline(s, 1000, '\n');
	}

	while (!file.eof())
	{
		int Id;
		int index;
		int x;
		int y;
		int width;
		int height;

		file >> Id >> index >> x >> y >> width >> height;

		ObjectTittle temp(Id, index, x * SCALE, y * SCALE, width * SCALE, height * SCALE);

		if (Id == 0) // for maptile
		{
			m_tileMapNode.push_back(temp);
		}
		else if (Id == 1) // stand position on map
		{
			m_StandPositionOnMap.push_back(Box(--x * SCALE, --y * SCALE, --width *SCALE, --height * SCALE));
		}
	}
	file.close();

	m_keyboard = keyboard;
}

MapObject::~MapObject()
{
	m_keyboard = NULL;
}


vector<Box> MapObject::getTileNodeOnScreen()
{
	vector<Box> list;
	Box camera(m_WorldPosition.x, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	for each (Box item in m_StandPositionOnMap)
	{
		if (CheckAABB(item, camera))
			list.push_back(Box(item));
	}

	return list;
}