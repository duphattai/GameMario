#include "Camera.h"

Camera* Camera::m_instance = 0;
Camera::Camera()
{
}
Camera::~Camera()
{
}

Camera* Camera::getInstance()
{
	if (m_instance == NULL)
		m_instance = new Camera();
	return m_instance;
}

void Camera::initialize(int vpx, int vpy, int screenWidth, int screenHeight, int worldWidth, int worldHeight)
{
	m_viewPortX = vpx; 
	m_viewPortY = vpy;
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_worldWidth = worldWidth;
	m_worldHeight = worldHeight;
}

void Camera::setViewPort(int vpx, int vpy)
{
	m_viewPortX = vpx;
	m_viewPortY = vpy;
}
Vector2 Camera::getViewport()
{
	return Vector2(m_viewPortX, m_viewPortY);
}

void Camera::setScreenWidth(int screenWidth)
{
	m_screenWidth = screenWidth;
}
int	Camera::getScreenWidth()
{
	return m_screenWidth;
}

void Camera::setScreenHeight(int screenHeight)
{
	m_screenHeight = screenHeight;
}
int	Camera::getScreenHeight()
{
	return m_screenHeight;
}


void Camera::setWorldHeight(int worldHeight)
{
	m_worldHeight = worldHeight;
}
int	Camera::getWorldHeight()
{
	return m_worldHeight;
}

void Camera::setWorldWidth(int worldWidth)
{
	m_worldWidth = worldWidth;
}
int	Camera::getWorldWidth()
{
	return m_worldWidth;
}


void Camera::update(Vector2 position)
{
	// update camera just move right
	if (position.x - m_viewPortX  > SCREEN_WIDTH / 2 && m_viewPortX + m_screenWidth < m_worldWidth)
		m_viewPortX = position.x - SCREEN_WIDTH / 2;
}

Box Camera::getCamera()
{
	Box camera(m_viewPortX, m_viewPortY - VIEW_PORT_Y, m_screenWidth, m_screenHeight);
	return camera;
}