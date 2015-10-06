#include "Sprite.h"


Sprite::Sprite()
{
	m_Index = 0;
}

Sprite::Sprite(const Sprite &copy)
{
	m_Count = copy.m_Count;
	m_Height = copy.m_Height;
	m_Image = copy.m_Image;
	m_Index = copy.m_Index;
	m_Rect = copy.m_Rect;
	m_SpritePerRow = copy.m_SpritePerRow;
	m_Width = copy.m_Width;
}

int Sprite::getHeight()
{
	return m_Height;
}

int Sprite::getWidth()
{
	return m_Width;
}

void Sprite::loadImageFromPath(LPDIRECT3DDEVICE9 d3ddv, LPCWSTR Path, int spriteperrow, int count, D3DXCOLOR color)
{
	HRESULT result;
	D3DXIMAGE_INFO info;
	result = D3DXGetImageInfoFromFile(Path, &info);
	if (result != D3D_OK)
	{
		exit(0);
		return;
	}

	result = D3DXCreateTextureFromFileEx(
		d3ddv,
		Path,
		info.Width,
		info.Height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		color,
		&info,
		NULL,
		&m_Image);

	if (result != D3D_OK)
	{
		MessageBox(0, L"Cannot load image", L"Error", MB_OK);
		return;
	}

	m_SpritePerRow = spriteperrow;
	m_Count = count;
	m_Width = info.Width / m_SpritePerRow;
	m_Height = info.Height / (m_Count / m_SpritePerRow);
	m_Index = 0;
}

void Sprite::loadImageFromPath(LPDIRECT3DDEVICE9 d3ddv, LPCWSTR Path, D3DXCOLOR color)
{
	HRESULT result;
	D3DXIMAGE_INFO info;
	result = D3DXGetImageInfoFromFile(Path, &info);
	if (result != D3D_OK)
	{
		exit(0);
		return;
	}

	result = D3DXCreateTextureFromFileEx(
		d3ddv,
		Path,
		info.Width,
		info.Height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		color,
		&info,
		NULL,
		&m_Image);

	if (result != D3D_OK)
	{
		MessageBox(0, L"Cannot load image", L"Error", MB_OK);
		return;
	}
}

void Sprite::next()
{
	m_Index++;
	if (m_Index > m_Count - 1) m_Index = 0;
}

// dùng để xét kích thước sprite cụ thể
void Sprite::setIndex(int value)
{
	m_Index = value;
	m_Rect.left = (m_Index % m_SpritePerRow) * m_Width;
	m_Rect.right = m_Rect.left + m_Width;
	m_Rect.top = (m_Index / m_SpritePerRow) * m_Height;
	m_Rect.bottom = m_Rect.top + m_Height;
}

// xét tọa độ cụ thể của sprite trong image sheet
void Sprite::setRect(RECT rect)
{
	m_Rect = rect;
	m_Width = abs(rect.right - rect.left);
	m_Height = abs(rect.top - rect.bottom);
}

// position: tọa độ vật trong thế giới thực
// scale: dùng để lật ảnh (-1.0f, 1.0f): lật ảnh, (1.0f, 1.0f) không lật
// vpx, vpy view port hay camera
void Sprite::draw(LPD3DXSPRITE SpriteHandler, D3DXVECTOR2 position, D3DXVECTOR2 scale, int vpx, int vpy)
{
	//
	// WORLD TO VIEWPORT TRANSFORM USING MATRIX
	//

	D3DXMATRIX mt;
	D3DXMatrixIdentity(&mt);
	mt._22 = -1.0f;
	mt._41 = (float)-vpx;
	mt._42 = (float)vpy;

	D3DXVECTOR4 vp_pos;
	D3DXVec3Transform(&vp_pos, &D3DXVECTOR3(position.x, position.y, 0), &mt);

	D3DXVECTOR3 p(vp_pos.x, vp_pos.y, 0);
	D3DXVECTOR3 center((float)m_Width/ 2, (float)m_Height/ 2, 0); // tâm của sprite trong textture


	D3DXMATRIX m_OldMatrix;
	SpriteHandler->GetTransform(&m_OldMatrix);

	D3DXMATRIX m_NewMatrix;
	D3DXMatrixTransformation2D(&m_NewMatrix, &D3DXVECTOR2(vp_pos.x, vp_pos.y), NULL, &scale, NULL, NULL, NULL);

	D3DXMATRIX m_FinalMatrix;
	m_FinalMatrix = m_OldMatrix * m_NewMatrix;


	/* Đã thay đổi hướng, tính chất của ảnh */
	SpriteHandler->SetTransform(&m_FinalMatrix);
	SpriteHandler->Draw(m_Image, &m_Rect, &center, &p, D3DCOLOR_XRGB(255, 255, 255));
	SpriteHandler->SetTransform(&m_OldMatrix);
}

int Sprite::getCount()
{
	return m_Count;
}
void Sprite::reset()
{
	m_Index = 0;
}

int Sprite::getCurrentIndex()
{
	return m_Index;
}

Sprite::~Sprite()
{
	m_Image->Release();
}
