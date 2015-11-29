#include "ScoreGame.h"
#include "ReSource.h"

ScoreGame* ScoreGame::m_instance = 0;

ScoreGame::ScoreGame()
{
	m_sprite = ReSource::getInstance()->getSprite(IDImage::IMG_ITEMSHEET);
	m_instance = 0;

	// xác định frameList cho count coin
	m_frameList.push_back(Frame(1, 160, 5, 8));
	//m_frameList.push_back(Frame(9, 160, 5, 8));
	//m_frameList.push_back(Frame(17, 160, 5, 8));
	m_width = 5;
	m_height = 8;

	m_currentFrame = 0;
	m_countCoin = 0;
	m_timeOfState = 0;
	m_score = 0;
	m_timeAnimation = 3;
}

ScoreGame* ScoreGame::getInstance()
{
	if (m_instance == NULL)
		m_instance = new ScoreGame();

	return m_instance;
}

bool ScoreGame::init(LPDIRECT3DDEVICE9 d3ddv)
{
	HRESULT result = D3DXCreateFont(d3ddv, 15, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Times New Roman"), &m_font);
	if (FAILED(result))
		return false;

	return true;
}

void ScoreGame::drawText(wstring text, Vector2 position)
{
	RECT rct;
	rct.left = position.x;
	rct.right = rct.left + 15 * text.length();
	rct.top = position.y;
	rct.bottom = position.y + 15;

	m_font->DrawText(NULL, text.c_str(), -1, &rct, 0, D3DCOLOR_XRGB(255, 255, 255));
}

void ScoreGame::draw(LPD3DXSPRITE spriteHandler)
{
	// dòng một
	drawText(L"MARIO", Vector2(24, 8));	
	drawText(L"WORLD", Vector2(144, 8));
	drawText(L"TIME", Vector2(200, 8));
	// dòng hai
	string text = to_string(m_score);
	{
		while (text.length() < 6)
			text = "0" + text;

		drawText(wstring(text.begin(), text.end()), Vector2(24, 18));
	}
	
	m_sprite->setRect(m_frameList[m_currentFrame].rect);
	m_sprite->draw(spriteHandler, D3DXVECTOR2(89 + m_width / 2, 210 + m_height / 2), D3DXVECTOR2(1.0f, 1.0f), 0, VIEW_PORT_Y, D3DCOLOR_XRGB(255, 255, 255));

	text = to_string(m_countCoin);
	while (text.length() < 2)
		text = "0" + text;
	text = "x" + text;
	drawText(wstring(text.begin(), text.end()), Vector2(95, 18));
	
	text = to_string(m_timeOfState);
	drawText(wstring(text.begin(), text.end()), Vector2(200, 16));
}

void ScoreGame::release()
{
	m_font->Release();
}