#pragma once
#include "Sprite.h"
#include "Global.h"
#include "GameTime.h"
#include "Collision.h"

class Collision;

class Object
{
protected:
	Sprite *m_Sprite; // con tro luu anh
	int m_IdImage; // loai hinh anh se duoc draw

	D3DXVECTOR2 m_Position; // toa do cua sprite tren map
	SpriteEffect m_Flip; // co lat hinh hay khong
	D3DXVECTOR2 m_Scale; // thong so lat hinh (1.0f, 1.0f) = khong flip, (-1.0f, 1.0f) = flip

	D3DXCOLOR	m_alpha; // độ trong suốt

	//Box m_Box;
	bool m_IsCollision;
	float m_TimeAnimation; // thoi gian de update su chuyen dong cua sprite
	
	GameTime *m_Gametime;

	Vector2 m_WorldPosition;

	Location	m_location;

	Vector2 m_Velocity;

	Collision	*m_CheckCollision;

	DIR			m_DirCollision;
public:
	Object();
	virtual ~Object();
	
	// set camera
	void			setWorldPosition(Vector2);
	Vector2			getWorldPosition();

	void			setVelocity(Vector2);
	Vector2			getVelocity();

	// set position on real world
	void			setPosition(float, float);
	Vector2			getPosition();

	virtual void	update() = 0;
	virtual void	draw(LPD3DXSPRITE);

	int				getHeight();
	int				getWidth();

	Location		getLocation();
	void			setLocation(int);

	bool			isCollision(Object *object);

	void			setDirCollision(DIR);
	DIR				getDirCollision();

	void			setFliping(SpriteEffect);
	SpriteEffect	getFliping();

	Box getBouding()
	{
		return Box(m_Position.x, m_Position.y, m_Sprite->getWidth() - 2, m_Sprite->getHeight(), m_Velocity.x, m_Velocity.y);
	}

	void			setAlphaColor(D3DXCOLOR);
	D3DXCOLOR		getAlphaColor(); // return white(1.0f, 1.0f, 1.0f)

	virtual void	updateVelocity(){};
};