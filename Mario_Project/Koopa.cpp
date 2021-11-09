#include "Koopa.h"

CKoopa::CKoopa(float x, float y, int type) : CGameObject(x, y, type)
{
	ax = 0;
	ay = KOOPA_SPEED_GRAVITY;
	SetState(KOOPA_STATE_WALKING);
	if (vx > 0)
		nx = 1;
	else
		nx = -1;
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx += ax * dt;
	vy += ay * dt;

	CCollision::GetInstance()->Process(this, dt, coObjects);
	//DebugOutTitle(L"Koopa: vx: %0.2f vy: %0.2f ax: %0.2f ay: %0.2f", vx, vy, ax, ay);
	DebugOutTitle(L"Koopa: nx: %d", nx);
}

int CKoopa::GetAniIdWalk()
{
	int ani_id = -1;
	
	if (nx > 0)
		ani_id = ID_ANI_KOOPA_WALKING_RIGHT;
	else
		ani_id = ID_ANI_KOOPA_WALKING_LEFT;

	if (ani_id == -1)
		ani_id = ID_ANI_KOOPA_WALKING_RIGHT;

	return ani_id;
}

int CKoopa::GetAniIdSpinShell()
{
	int ani_id = -1;

	if (nx > 0)
		ani_id = ID_ANI_KOOPA_SPIN_SHELL_RIGHT;
	else
		ani_id = ID_ANI_KOOPA_SPIN_SHELL_LEFT;

	if (ani_id == -1)
		ani_id = ID_ANI_KOOPA_SPIN_SHELL_RIGHT;

	return ani_id;
}

void CKoopa::Render()
{
	int ani_id = ID_ANI_KOOPA_SPIN_SHELL_RIGHT;

	if (GetState() == KOOPA_STATE_WALKING)
		ani_id = GetAniIdWalk();
	else if (GetState() == KOOPA_STATE_SHELLING)
		ani_id = ID_ANI_KOOPA_SHELLING;
	else if (GetState() == KOOPA_STATE_SPIN_SHELL)
		ani_id = GetAniIdSpinShell();

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

	if (e->nx != 0) // Nếu va chạm với Blocking object theo trục x, thì quay đầu lại và đi tiếp (cho vận tốc và normal vector ngược dấu)
	{
		this->vx = -vx;
		this->nx = -nx;
	}
}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
		case KOOPA_STATE_WALKING:
		{
			vx = -KOOPA_SPEED_WALKING;
			break;
		}
		case KOOPA_STATE_SHELLING:
		{
			vx = 0;
			break;
		}
		case KOOPA_STATE_SPIN_SHELL:
		{
			if (GetNormalDirectionX() == DIRECTION_RIGHT)
				vx = KOOPA_SPEED_SPINNING;
			else
				vx = -KOOPA_SPEED_SPINNING;

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