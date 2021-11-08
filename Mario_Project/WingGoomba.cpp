#include "WingGoomba.h"

CWingGoomba::CWingGoomba(float x, float y, int type, float walking_distance) : CGoomba(x, y, type)
{
	this->ax = 0;
	this->ay = WING_GOOMBA_GRAVITY;

	this->walkingDistance = walking_distance;
	this->startWalkingLocation = x;
}



void CWingGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx += ax * dt;
	vy += ay * dt;

	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CWingGoomba::OnNoCollision(DWORD dt)
{
	/*x += vx * dt;
	y += vy * dt;*/
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
	int ani_id = ID_ANI_WING_GOOMBA_FLYING;

	CAnimations::GetInstance()->Get(ani_id)->Render(x, y);
}

void CWingGoomba::SetState(int state)
{
}

void CWingGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - WING_GOOMBA_BBOX_WIDTH / 2;
	top = y - WING_GOOMBA_BBOX_HEIGHT / 2;
	right = left + WING_GOOMBA_BBOX_WIDTH;
	bottom = top + WING_GOOMBA_BBOX_HEIGHT;
}