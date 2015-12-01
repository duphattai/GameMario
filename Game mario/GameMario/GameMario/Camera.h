#pragma once
#include "Global.h"
class Camera
{
	int		m_viewPortX;
	int		m_viewPortY;
	int		m_worldWidth;
	int		m_worldHeight;

	int		m_screenWidth;
	int		m_screenHeight;

	Camera();
	static Camera*  m_instance;
public:
	static Camera* getInstance();

	void			initialize(int vpx, int vpy, int screenWidth, int screenHeight, int worldWidth, int worldHeight);

	Box				getCamera();

	Vector2			getViewport();
	void			setViewPort(int, int);

	void			setScreenWidth(int);
	int				getScreenWidth();

	void			update(Vector2);

	void			setScreenHeight(int);
	int				getScreenHeight();

	void			setWorldWidth(int);
	int				getWorldWidth();

	void			setWorldHeight(int);
	int				getWorldHeight();

	~Camera();
};

