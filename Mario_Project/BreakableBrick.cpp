#include "BreakableBrick.h"

void CBreakableBrick::Render()
{
	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);
	float width = right - left;
	float height = bottom - top;

	CAnimations::GetInstance()->Get(ID_ANI_BREAKABLE_BRICK)->Render(x + width / 2, y + height / 2);
	//RenderBoundingBox();
}

void CBreakableBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BREAKABLE_BRICK_BBOX_WIDTH;
	bottom = top + BREAKABLE_BRICK_BBOX_HEIGHT;
}

void CBreakableBrick::SetState(int state)
{
}
