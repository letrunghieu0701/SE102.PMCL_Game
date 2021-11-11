#include "Pipe.h"

void CPipe::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_PIPE)->Render(x, y);
}

void CPipe::Update(DWORD dt)
{
}

void CPipe::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - PIPE_BBOX_WIDTH / 2;
	top = y - PIPE_BBOX_HEIGHT / 2;
	right = left + PIPE_BBOX_WIDTH;
	bottom = top + PIPE_BBOX_HEIGHT;
}

void CPipe::SetState(int state)
{
	CGameObject::SetState(state);
}
