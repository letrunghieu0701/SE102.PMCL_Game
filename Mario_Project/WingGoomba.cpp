#include "WingGoomba.h"

CWingGoomba::CWingGoomba(float x, float y, int type, float walking_distance) : CGoomba(x, y, type)
{
	this->ax = 0;
	this->ay = WING_GOOMBA_GRAVITY;

	this->walkingDistance = walking_distance;
	this->startWalkingLocation = x;

	SetState(WING_GOOMBA_STATE_WALKING);
}



void CWingGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx += ax * dt;
	vy += ay * dt;

	// Nếu Wing Goomba chết thì xóa và hủy cập nhật
	if (GetState() == WING_GOOMBA_STATE_DIE &&
		GetTickCount64() - die_start > WING_GOOMBA_DIE_TIMEOUT)
	{
		isDeleted = true;
		return;
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
	//DebugOutTitle(L"WingGoomba: vx: %0.2f vy: %0.2f ax: %0.2f ay: %0.2f", vx, vy, ax, ay);
	DebugOutTitle(L"State: %d", state);
}


void CWingGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;	
}

void CWingGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	
	if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CWingGoomba::Render()
{
	int ani_id = ID_ANI_WING_GOOMBA_WALKING;

	if (state == WING_GOOMBA_STATE_WALKING)
		ani_id = ID_ANI_WING_GOOMBA_WALKING;
	else if (state == WING_GOOMBA_STATE_FLYING)
		ani_id = ID_ANI_WING_GOOMBA_FLYING;
	else
		ani_id = ID_ANI_WING_GOOMBA_DIE;

	CAnimations::GetInstance()->Get(ani_id)->Render(x, y);
}

void CWingGoomba::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case WING_GOOMBA_STATE_DIE:
		die_start = GetTickCount64();
		y += (WING_GOOMBA_BBOX_HEIGHT - WING_GOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case WING_GOOMBA_STATE_WALKING:
		vx = WING_GOOMBA_SPEED_WALKING;
		break;
	}
}

void CWingGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - WING_GOOMBA_BBOX_WIDTH / 2;
	top = y - WING_GOOMBA_BBOX_HEIGHT / 2;
	right = left + WING_GOOMBA_BBOX_WIDTH;
	bottom = top + WING_GOOMBA_BBOX_HEIGHT;
}