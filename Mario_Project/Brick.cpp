#include "Brick.h"
#include "debug.h"

void CBrick::Render()
{
	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);
	float width = right - left + 1;
	float height = bottom - top + 1;

	CAnimations::GetInstance()->Get(ID_ANI_BRICK)->Render(x + width/2 - 1 , y + height/2 - 1);
	RenderBoundingBox();
}

void CBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BRICK_BBOX_WIDTH;
	bottom = top + BRICK_BBOX_HEIGHT;
}

void CBrick::SetState(int state)
{
	CGameObject::SetState(state);
}
