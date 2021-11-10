#include "ChangeDirectionOnPlatform.h"


CChangeDirectionOnPlatform::CChangeDirectionOnPlatform(float x, float y, int type) : CGameObject(x, y, type)
{
}

void CChangeDirectionOnPlatform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}


void CChangeDirectionOnPlatform::OnNoCollision(DWORD dt)
{
}

void CChangeDirectionOnPlatform::OnCollisionWith(LPCOLLISIONEVENT e)
{
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