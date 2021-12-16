#include "WingGoomba.h"

CWingGoomba::CWingGoomba(float x, float y, int type, float walking_distance) : CGoomba(x, y, type)
{
	this->ax = 0;
	this->ay = WING_GOOMBA_GRAVITY;

	this->level = WING_GOOMBA_LEVEL_HAVE_WING;
	this->walkingDistance = walking_distance;
	this->startWalkingLocation = x;

	this->nx = -1;
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

	// Nếu level "có cánh" và 
	// đang phải đang đi bộ (state walking) (vì nếu đang bay mà di chuyển lớn hơn quãng đường cần bay thì cũng có thể kích hoạt việc bay
	// và phải đi hết một quãng đường quy định
	// thì mới cho bay (state flying)
	if (GetLevel() == WING_GOOMBA_LEVEL_HAVE_WING &&
		GetState() == WING_GOOMBA_STATE_WALKING &&
		abs(x - startWalkingLocation) > walkingDistance)
	{
		SetState(WING_GOOMBA_STATE_FLYING);
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
	//DebugOutTitle(L"WingGoomba: vx: %0.2f vy: %0.2f ax: %0.2f ay: %0.2f", vx, vy, ax, ay);
	//DebugOutTitle(L"Wing Goomba: State: %d Level: %d", state, level);
	//DebugOutTitle(L"Wing Goomba: %d", level);
}


void CWingGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;	
}

void CWingGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->IsBlocking())
	{
		if (e->ny != 0)
		{
			vy = 0;
		}
		

		// Nếu Wing Goomba rớt xuống mặt đất thì đặt lại startWalkingLocation để lát nữa Wing Goomba có thể bay
		if (e->ny < 0 &&
			// Phải đang ở state flying thì mới đặt lại thành walking được,
			// vì nếu cứ chạm đất thì sẽ luôn đặt lại startWalkingLocation, mà như vậy thì khoảng cách mới và
			// startWalkingLocation sẽ rất gần nhau nếu Wing Goomba di chuyển chậm, dẫn đến không thể chuyển sang state flying được
			GetState() == WING_GOOMBA_STATE_FLYING)	
		{
			SetState(WING_GOOMBA_STATE_WALKING);
		}

		/*if (e->ny < 0)
		{
			SetState(WING_GOOMBA_STATE_WALKING);
		}*/

		if (e->nx != 0)
		{
			vx = -vx;
			if (vx > 0)
			{
				this->SetNormalDirectionX(DIRECTION_RIGHT);
			}
			else
			{
				this->SetNormalDirectionX(DIRECTION_LEFT);
			}
		}
	}
}


int CWingGoomba::GetAniIdHaveWing()
{
	int ani_id = -1;

	if (ay == WING_GOOMBA_GRAVITY_WHEN_FLYING)
		ani_id = ID_ANI_WING_GOOMBA_HAVE_WING_FLYING;
	else if (ay == WING_GOOMBA_GRAVITY)
		ani_id = ID_ANI_WING_GOOMBA_HAVE_WING_WALKING;

	if (ani_id == -1)
		ani_id = ID_ANI_WING_GOOMBA_HAVE_WING_WALKING;

	return ani_id;
}

int CWingGoomba::GetAniIdNoWing()
{
	return ID_ANI_WING_GOOMBA_NO_WING_WALKING;
}

void CWingGoomba::Render()
{
	int ani_id = ID_ANI_WING_GOOMBA_HAVE_WING_WALKING;

	if (state == WING_GOOMBA_STATE_DIE)
		ani_id = ID_ANI_WING_GOOMBA_DIE;
	else
	{
		if (GetLevel() == WING_GOOMBA_LEVEL_HAVE_WING)
			ani_id = GetAniIdHaveWing();
		else
			ani_id = GetAniIdNoWing();
	}
		
	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);
	float width = right - left;
	float height = bottom - top;

	CAnimations::GetInstance()->Get(ani_id)->Render(x + width / 2, y + height / 2);
	//CAnimations::GetInstance()->Get(ani_id)->Render(x, y);
	RenderBoundingBox();
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
		// Nếu có cánh (level có-cánh) thì mới bắt đầu "tính khoảng cách" để lát nữa bay 
		if (this->GetLevel() == WING_GOOMBA_LEVEL_HAVE_WING)
			startWalkingLocation = x;

		// Đặt lại 2 thông số sau để thể hiện là Wing Goomba đang di chuyển bình thường trên mặt đất
		if (this->GetNormalDirectionX() == DIRECTION_RIGHT)
		{
			vx = WING_GOOMBA_SPEED_WALKING;
		}
		else
		{
			vx = -WING_GOOMBA_SPEED_WALKING;
		}
		ay = WING_GOOMBA_GRAVITY;
		break;
	case WING_GOOMBA_STATE_FLYING:
		vy = -WING_GOOMBA_SPEED_FLYING;
		ay = WING_GOOMBA_GRAVITY_WHEN_FLYING;	// Giảm gia tốc ay để tạo cảm giác hạ cánh từ từ
		// Giảm vận tốc vx để tạo cảm giác có lực cản không khí trên cao nên không thể di chuyển theo trục x nhanh được
		if (this->GetNormalDirectionX() == DIRECTION_RIGHT)
		{
			vx = WING_GOOMBA_SPEED_WALKING_WHEN_FLYING;
		}
		else
		{
			vx = -WING_GOOMBA_SPEED_WALKING_WHEN_FLYING;
		}
		
		break;
	}
}

void CWingGoomba::SetLevel(int l)
{
	this->level = l;
}

int CWingGoomba::GetLevel()
{
	return this->level;
}

void CWingGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (GetState() == WING_GOOMBA_STATE_DIE)
	{
		left = x;
		top = y;
		right = left + WING_GOOMBA_BBOX_WIDTH;
		bottom = top + WING_GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x;
		top = y;
		right = left + WING_GOOMBA_BBOX_WIDTH;
		bottom = top + WING_GOOMBA_BBOX_HEIGHT;
	}
}