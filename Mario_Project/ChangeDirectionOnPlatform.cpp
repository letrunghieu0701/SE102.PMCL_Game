#include "ChangeDirectionOnPlatform.h"


CChangeDirectionOnPlatform::CChangeDirectionOnPlatform(float x, float y, int type) : CGameObject(x, y, type)
{
	this->ay = CDOP_SPEED_GRAVITY;
}

void CChangeDirectionOnPlatform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;

	CCollision::GetInstance()->Process(this, dt, coObjects);
	//DebugOutTitle(L"CDOP: x: %d y: %d", x, y);
	DebugOutTitle(L"CDOP: vx: %0.2f vy: %0.2f", vx, vy);
}


void CChangeDirectionOnPlatform::OnNoCollision(DWORD dt)
{
	y += vy * dt;
}

void CChangeDirectionOnPlatform::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->IsBlocking())
	{
		this->vy = 0;
	}
}

void CChangeDirectionOnPlatform::Render()
{
	int ani_id = ID_ANI_CDOP;

	CAnimations::GetInstance()->Get(ani_id)->Render(x, y);
}

void CChangeDirectionOnPlatform::SetState(int state)
{
	CGameObject::SetState(state);
}


void CChangeDirectionOnPlatform::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - CDOP_BBOX_WIDTH / 2;
	top = y - CDOP_BBOX_HEIGHT / 2;
	right = left + CDOP_BBOX_WIDTH;
	bottom = top + CDOP_BBOX_HEIGHT;
}