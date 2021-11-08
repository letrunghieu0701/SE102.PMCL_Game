#include "WingGoomba.h"

void CWingGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - WING_GOOMBA_BBOX_WIDTH / 2;
	top = y - WING_GOOMBA_BBOX_HEIGHT / 2;
	right = left + WING_GOOMBA_BBOX_WIDTH;
	bottom = top + WING_GOOMBA_BBOX_HEIGHT;
}

void CWingGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CWingGoomba::Render()
{
	int ani_id = ID_ANI_WING_GOOMBA_WALKING;

	CAnimations::GetInstance()->Get(ani_id)->Render(x, y);
}

void CWingGoomba::OnNoCollision(DWORD dt)
{
}

void CWingGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CWingGoomba::SetState(int state)
{
}
