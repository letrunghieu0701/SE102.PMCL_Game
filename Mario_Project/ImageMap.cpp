#include "ImageMap.h"


void CImageMap::Render()
{
	CSprite* s = CSprites::GetInstance()->Get(this->spriteId);

	float l, t, r, b;
	GetBoundingBox(l, t, r, b);
	float re_position_x = x + (r - l) / 2;
	float re_position_y = y + (b - t) / 2;
	s->Draw(re_position_x, re_position_y);
}

void CImageMap::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	CSprite* s = CSprites::GetInstance()->Get(this->spriteId);
	
	l = s->GetLeft();
	t = s->GetTop();
	r = s->GetRight();
	b = s->GetBottom();
}


