#include "MapObject.h"


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

		ObjectTittle temp(Id, index, x, y, width, height);

		if (Id == 0) // for maptile
		{
			m_tileMapNode.push_back(temp);
		}
		
	}
	file.close();

	m_keyboard = keyboard;
}

MapObject::~MapObject()
{
	m_keyboard = NULL;
}
