#include "Koopa.h"

#include "Goomba.h"
#include "WingGoomba.h"

CKoopa::CKoopa(float x, float y, int type) : CGameObject(x, y, type)
{
	ax = 0;
	ay = KOOPA_SPEED_GRAVITY;
	SetState(KOOPA_STATE_WALKING);
	if (vx > 0)
		nx = DIRECTION_RIGHT;
	else
		nx = DIRECTION_LEFT;
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
	if (e->obj->IsBlocking())
	{
		if (e->ny != 0) // Va chạm với Blocking object theo trục y, thì đặt lại vy = 0 để mô phỏng vật lý cơ bản
		{
			this->vy = 0;
		}
		else if (e->nx != 0) // Nếu va chạm với Blocking object theo trục x, thì quay đầu lại và đi tiếp (cho vận tốc và normal vector ngược dấu)
		{
			this->vx = -vx;
			this->nx = -nx;
		}
	}

	if (e->obj->GetType() == OBJECT_TYPE_GOOMBA)
		OnCollisionWithGoomba(e);
	else if (e->obj->GetType() == OBJECT_TYPE_WING_GOOMBA)
		OnCollisionWithWingGoomba(e);
}

void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	// Koopa đang ở state Spin Shell thì mới có thể tấn công Goomba được
	if (this->GetState() == KOOPA_STATE_SPIN_SHELL)
	{
		CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

		// Nếu Goomba đang ở trạng thái khác DIE, tức là Goomba còn sống, thì cho nó die luôn
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
		}
		// Còn nếu Goomba đang ở state DIE, thì thôi, không làm gì cả, vì nó đang ở state DIE rồi mà
	}
}

void CKoopa::OnCollisionWithWingGoomba(LPCOLLISIONEVENT e)
{
	// Koopa đang ở state Spin Shell thì mới có thể tấn công Wing Goomba được
	if (this->GetState() == KOOPA_STATE_SPIN_SHELL)
	{
		CWingGoomba* wing_goomba = dynamic_cast<CWingGoomba*>(e->obj);

		// Nếu Wing Goomba còn 2 mạng (level có-cánh), thì giảm một mạng (level thành không-cánh) và gắn state thành WALKING (vì đâu có còn cánh đâu mà bay)
		if (wing_goomba->GetLevel() == WING_GOOMBA_LEVEL_HAVE_WING)
		{
			wing_goomba->SetLevel(WING_GOOMBA_LEVEL_NO_WING);
			wing_goomba->SetState(WING_GOOMBA_STATE_WALKING);
		}
		// Nếu Wing Goomba chỉ còn 1 mạng (level không-cánh), thì cho die luôn
		else if (wing_goomba->GetLevel() == WING_GOOMBA_LEVEL_NO_WING)
		{
			wing_goomba->SetState(WING_GOOMBA_STATE_DIE);
		}
		// Nếu Wing Goomba đã die rồi, thì thôi, không làm gì cả, vì nó DIE rồi mà
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