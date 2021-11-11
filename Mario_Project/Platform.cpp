#include "Platform.h"

#include "Sprite.h"
#include "Sprites.h"



void CPlatform::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
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

	//float xx = x - this->cellWidth / 2 + rect.right / 2;

	CGame::GetInstance()->Draw(x + rect.right/2- cx, y + rect.bottom / 2 - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}


void CPlatform::Render()
{
	if (this->length <= 0) return;

	float xx = x + this->cellWidth / 2;
	float new_y = y + this->cellHeight / 2;
	CSprites * s = CSprites::GetInstance();

	s->Get(this->spriteIdBegin)->Draw(xx, new_y);
	xx += this->cellWidth;
	for (int i = 1; i < this->length - 1; i++)
	{
		s->Get(this->spriteIdMiddle)->Draw(xx, new_y);
		xx += this->cellWidth;
	}
	if (length>1)
		s->Get(this->spriteIdEnd)->Draw(xx, new_y);

	//
	RenderBoundingBox();
	//

}

void CPlatform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + this->cellWidth * this->length;
	b = t + this->cellHeight;
}