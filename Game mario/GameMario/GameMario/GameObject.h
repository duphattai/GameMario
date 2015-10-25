#pragma once
#include "Sprite.h"
#include "Global.h"
#include "Collision.h"

class Collision;

class GameObject
{
protected:
	Sprite*		m_Sprite; // con tro luu anh
	int			m_index; // rất cần thiết


	D3DXVECTOR2		m_Position; // toa do cua sprite tren map
	SpriteEffect	m_Flip; // co lat hinh hay khong
	D3DXVECTOR2		m_Scale; // thong so lat hinh (1.0f, 1.0f) = khong flip, (-1.0f, 1.0f) = flip

	D3DXCOLOR		m_alpha; // độ trong suốt

	Box				m_Box; // for object not have image
	
	bool			m_IsCollision;
	float			m_TimeAnimation; // thoi gian de update su chuyen dong cua sprite

	Vector2			m_WorldPosition;

	Location		m_location;

	Vector2			m_Velocity;

	TypeObject		m_typeObject;

	DIR				m_DirCollision;

	Collision*		m_checkCollision;
public:
	GameObject();
	virtual ~GameObject();
	
	//
	void			setSpriteSheet(Sprite*);

	//
	void			setIndexSprite(int);
	int				getIndexSprite();


	// set camera
	void			setWorldPosition(Vector2);
	Vector2			getWorldPosition();

	void			setVelocity(Vector2);
	Vector2			getVelocity();

	// set position on real world
	virtual void	setPosition(float, float);
	Vector2			getPosition();

	virtual void	update(){};
	virtual void	draw(LPD3DXSPRITE);

	int				getHeight();
	int				getWidth();

	Location		getLocation();
	void			setLocation(int);

	virtual bool	isCollision(GameObject*){ return true; }

	void			setDirCollision(DIR);
	DIR				getDirCollision();

	void			setFliping(SpriteEffect);
	SpriteEffect	getFliping();

	virtual Box		getBouding();

	void			setTypeObject(TypeObject);
	TypeObject		getTypeObject();

	Box				getBox();
	void			setBox(Box);


	void			setAlphaColor(D3DXCOLOR);
	D3DXCOLOR		getAlphaColor(); // return white(1.0f, 1.0f, 1.0f)

	virtual void	updateVelocity(){};
};