#include "Pipe.h"

void CPipe::Render()
{
	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);
	float width = right - left;
	float height = bottom - top;

	CAnimations::GetInstance()->Get(ID_ANI_PIPE)->Render(x + width / 2, y + height / 2);
	RenderBoundingBox();
}

void CPipe::Update(DWORD dt)
{
}

void CPipe::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + PIPE_BBOX_WIDTH;
	bottom = top + PIPE_BBOX_HEIGHT;
}

void CPipe::SetState(int state)
{
	CGameObject::SetState(state);
}
