#include "Koopa.h"

CKoopa::CKoopa(float x, float y, int type) : CGameObject(x, y, type)
{
	ax = 0;
	ay = KOOPA_SPEED_GRAVITY;
	SetState(KOOPA_STATE_WALKING);
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx += ax * dt;
	vy += ay * dt;

	CCollision::GetInstance()->Process(this, dt, coObjects);
	DebugOutTitle(L"Koopa: vx: %0.2f vy: %0.2f ax: %0.2f ay: %0.2f", vx, vy, ax, ay);
}

void CKoopa::Render()
{
	int ani_id = ID_ANI_KOOPA_WALKING_LEFT;

	CAnimations::GetInstance()->Get(ani_id)->Render(x, y);
	RenderBoundingBox();
}

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;

	if (e->ny != 0) // Va chạm với Blocking object theo trục y, thì đặt lại vy = 0 để mô phỏng vật lý cơ bản
	{
		this->vy = 0;
	}

	if (e->nx != 0) // Nếu va chạm với Blocking object theo trục x, thì quay đầu lại và đi tiếp
	{
		this->vx = -vx;
	}
}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
		case KOOPA_STATE_DIE:
		{
			vx = 0;
			break;
		}
		case KOOPA_STATE_WALKING:
		{
			vx = KOOPA_SPEED_WALKING;
			break;
		}
		case KOOPA_STATE_SHELLING:
		{
			vx = 0;
			break;
		}
		case KOOPA_STATE_SPIN_SHELL:
		{
			vx = KOOPA_SPEED_SPINNING;
			break;
		}
	}
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - KOOPA_BBOX_WIDTH / 2;
	top = y - KOOPA_BBOX_HEIGHT / 2;
	right = left + KOOPA_BBOX_WIDTH;
	bottom = top + KOOPA_BBOX_HEIGHT;
}