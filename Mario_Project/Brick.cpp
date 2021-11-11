#include "Brick.h"

void CBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_BRICK)->Render(x, y);
	RenderBoundingBox();
}

void CBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - BRICK_BBOX_WIDTH/2;
	top = y - BRICK_BBOX_HEIGHT/2;
	right = left + BRICK_BBOX_WIDTH;
	bottom = top + BRICK_BBOX_HEIGHT;
}

void CBrick::SetState(int state)
{
	CGameObject::SetState(state);
}
