#include "ImageMap.h"


void CImageMap::Render()
{
	CSprite* s = CSprites::GetInstance()->Get(this->spriteId);

	float l, t, r, b;
	GetBoundingBox(l, t, r, b);
	float re_position_x = x - (TILE_WIDTH / 2) + ((r - l) / 2);
	float re_position_y = y - (TILE_HEIGHT / 2) + ((b - t) / 2);
	s->Draw(re_position_x, re_position_y);
	//s->Draw(x, y);

	//CAnimations::GetInstance()->Get(ID_ANI_IMAGE_MAP)->Render(x, y);
}

void CImageMap::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	CSprite* s = CSprites::GetInstance()->Get(this->spriteId);
	left = s->GetLeft();
	top = s->GetTop();
	right = s->GetRight();
	bottom = s->GetBottom();

	/*left = x - IMAGE_MAP_BBOX_WIDTH / 2;
	top = y - IMAGE_MAP_BBOX_HEIGHT / 2;
	right = left + IMAGE_MAP_BBOX_WIDTH;
	bottom = top + IMAGE_MAP_BBOX_HEIGHT;*/

	/*left = x;
	top = y;
	right = left + IMAGE_MAP_BBOX_WIDTH;
	bottom = top + IMAGE_MAP_BBOX_HEIGHT;*/
}


