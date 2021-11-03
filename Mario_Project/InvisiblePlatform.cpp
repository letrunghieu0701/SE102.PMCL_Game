#include "InvisiblePlatform.h"

void CInvisiblePlatform::Render()
{
	/*DebugOutTitle(L"Invis: %0.2f, %0.2f", x, y);*/
	RenderBoundingBox();
}

void CInvisiblePlatform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - (width / 2);
	t = y - (height / 2);
	r = l + width;
	b = t + height;
}

void CInvisiblePlatform::RenderBoundingBox()
{
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	float xx = x - this->width / 2 + rect.right / 2;

	CGame::GetInstance()->Draw(xx - cx, y - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}
