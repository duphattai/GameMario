#pragma once
#include "Sprite.h"
#include "Global.h"
#include "Collision.h"
#include <string>

class Collision;

class GameObject
{
protected:
	Sprite*		m_sprite; // con tro luu anh
	int			m_index; // rất cần thiết

	wstring			m_text; // draw text

	D3DXVECTOR2		m_position; // toa do cua sprite tren map
	SpriteEffect	m_flip; // co lat hinh hay khong
	D3DXVECTOR2		m_scale; // thong so lat hinh (1.0f, 1.0f) = khong flip, (-1.0f, 1.0f) = flip

	D3DXCOLOR		m_alpha; // độ trong suốt

	Box				m_box; // for object not have image
	
	bool			m_isActive;

	StatusObject	m_status;

	bool			m_isCollision;
	int			m_timeAnimation; // thoi gian de update su chuyen dong cua sprite

	Vector2			m_worldPosition;

	Location		m_location;

	Vector2			m_velocity;

	TypeObject		m_typeObject;

	DIR				m_dirCollision;

	Collision*		m_checkCollision;
public:
	GameObject();
	virtual ~GameObject();
	
	//
	void			setSpriteSheet(Sprite*);

	//
	void			setIndexSprite(int);
	int				getIndexSprite();

	void			setTimeAnimation(int x){ m_timeAnimation = x; }
	int				getTimeAnimation(){ return m_timeAnimation; }

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
	virtual void	drawText(wstring text, Vector2 position);


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

	void			setStatusObject(StatusObject);
	StatusObject	getStatusOBject();

	Box				getBox();
	void			setBox(Box);

	void			setText(wstring);

	void			setActive(bool x);
	bool			isActive();

	void			setAlphaColor(D3DXCOLOR);
	D3DXCOLOR		getAlphaColor(); // return white(1.0f, 1.0f, 1.0f)

	virtual void	updateVelocity(){};
};