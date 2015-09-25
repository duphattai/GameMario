#pragma once
#include "Sprite.h"
#include "Global.h"
#include "GameTime.h"

class Object
{
protected:
	Sprite *m_Sprite; // con tro luu anh
	int m_IdImage; // loai hinh anh se duoc draw

	D3DXVECTOR2 m_Position; // toa do cua sprite tren map
	SpriteEffect m_Flip; // co lat hinh hay khong
	D3DXVECTOR2 m_Scale; // thong so lat hinh (1.0f, 1.0f) = khong flip, (-1.0f, 1.0f) = flip

	//Box m_Box;
	bool m_IsCollision;
	float m_TimeAnimation; // thoi gian de update su chuyen dong cua sprite
	
	GameTime *m_Gametime;

	Vector2 m_WorldPosition;

	Vector2 m_Velocity;
public:
	Object();

	void		setWorldPosition(Vector2);
	Vector2		getWorldPosition();

	void		setVelocity(Vector2);
	Vector2		getVelocity();

	void		setPosition(int x, int y);
	Vector2		getPosition();

	virtual void update() = 0;
	virtual void draw(LPD3DXSPRITE);

	int getHeight()
	{
		return m_Sprite->getHeight();
	}

	int getWidth()
	{
		return m_Sprite->getWidth();
	}

	bool isCollision(Object *object);

	void setFliping(SpriteEffect efect){ m_Flip = efect; }

	Box getBouding()
	{
		return Box(m_Position.x, m_Position.y, m_Sprite->getWidth(), m_Sprite->getHeight(), m_Velocity.x, m_Velocity.y);
	}

	virtual void updateVelocity(){};

	virtual ~Object();
};