#pragma once
#include "GameObject.h"
#include <vector>
class ScoreGame
{
	int			m_width;
	int			m_height;

	Sprite*		m_sprite;
	LPD3DXFONT	m_font;
	int			m_currentFrame;
	std::vector<Frame> m_frameList;

	int			m_timeAnimation;
	int			m_timeOfState;
	int			m_countCoin;
	int			m_score;

	ScoreGame();
	static ScoreGame* m_instance;
public:
	~ScoreGame(){};
	static ScoreGame* getInstance();

	void    release();
	bool	init(LPDIRECT3DDEVICE9);
	void	draw(LPD3DXSPRITE);

	virtual void	drawText(wstring text, Vector2 position);

	int		getTimeOfState(){ return m_timeOfState; }
	void	setTimeOfState(int time){ m_timeOfState = time; }

	int		getCountOfCoin(){ return m_countCoin; }
	void	setCountOfCoin(int coin){ m_countCoin = coin; }

	int		getScore(){ return m_score; }
	void	setScore(int score){ m_score = score; }
};

