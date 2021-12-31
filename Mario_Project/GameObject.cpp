#include <d3dx9.h>
#include <algorithm>


#include "debug.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;	
	state = -1;
	isDeleted = false;
	type = -1;
}

void CGameObject::RenderBoundingBox()
{
	// Hàm này dùng GetBounding để lấy kích thước cần thiết của vật thể, kích thước này sẽ là kích thước của bbox luôn
	// VD của kích thước cần thiết:
	// + Mario Big: lấy hết cả width và height của sprite luôn, cũng đồng thời là kích thước thật sự cần thiết của Mario
	// + Mario Racccoon: Bỏ phần kích thước dư của đuôi, chỉ lấy phần kích thước của Mario mà thôi

	// Còn vị trí vẽ bbox vẫn là vị trí của vật thể, do là tất cả sprite bị vẽ từ center của sprite
	// nên cần vẽ bbox (sprite) ở vị trí center của object để có thể trùng với vị trí của animation (nên vẽ ra giấy để dễ hình dung)
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l,t,r,b; 

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy; 
	CGame::GetInstance()->GetCamPos(cx, cy);

	float draw_pos_x = x + rect.right / 2 - cx;
	float draw_pos_y = y + rect.bottom / 2 - cy;
		
	CGame::GetInstance()->Draw(draw_pos_x, draw_pos_y, bbox, &rect, BBOX_ALPHA);

}

CGameObject::~CGameObject()
{

}