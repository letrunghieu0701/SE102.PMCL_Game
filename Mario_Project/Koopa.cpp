#include "Koopa.h"

CKoopa::CKoopa(float x, float y, int type) : CGameObject(x, y, type)
{

}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}

void CKoopa::Render()
{
	int ani_id = ID_ANI_KOOPA_WALKING_LEFT;

	CAnimations::GetInstance()->Get(ani_id)->Render(x, y);
	RenderBoundingBox(); 
}

void CKoopa::OnNoCollision(DWORD dt)
{
}

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
}


void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - KOOPA_BBOX_WIDTH / 2;
	top = y - KOOPA_BBOX_HEIGHT / 2;
	right = left + KOOPA_BBOX_WIDTH;
	bottom = top + KOOPA_BBOX_HEIGHT;
}