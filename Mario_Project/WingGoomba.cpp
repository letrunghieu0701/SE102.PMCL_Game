#include "WingGoomba.h"

CWingGoomba::CWingGoomba(float x, float y, int type, float walking_distance) : CGoomba(x, y, type)
{
	this->ax = 0;
	this->ay = WING_GOOMBA_GRAVITY;

	this->level = WING_GOOMBA_LEVEL_HAVE_WING;
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

	if (GetState() == WING_GOOMBA_STATE_WALKING &&
		x - startWalkingLocation > walkingDistance)
	{
		SetState(WING_GOOMBA_STATE_FLYING);
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
	//DebugOutTitle(L"WingGoomba: vx: %0.2f vy: %0.2f ax: %0.2f ay: %0.2f", vx, vy, ax, ay);
	//DebugOutTitle(L"State: %d", state);
	DebugOutTitle(L"Wing Goomba: %d", level);
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

		// Nếu Wing Goomba rớt xuống mặt đất thì đặt lại startWalkingLocation để lát nữa Wing Goomba có thể bay
		if (e->ny < 0 &&
			// Phải đang ở state flying thì mới đặt lại thành walking được,
			// vì nếu cứ chạm đất thì sẽ luôn đặt lại startWalkingLocation, mà như vậy thì khoảng cách mới và
			// startWalkingLocation sẽ rất gần nhau nếu Wing Goomba di chuyển chậm, dẫn đến không thể chuyển sang state flying được
			GetState() == WING_GOOMBA_STATE_FLYING)	
		{
			SetState(WING_GOOMBA_STATE_WALKING);
		}
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
		startWalkingLocation = x;

		// Đặt lại 2 thông số sau để thể hiện là Wing Goomba đang di chuyển bình thường trên mặt đất
		vx = WING_GOOMBA_SPEED_WALKING;
		ay = WING_GOOMBA_GRAVITY;
		break;
	case WING_GOOMBA_STATE_FLYING:
		vy = -WING_GOOMBA_SPEED_FLYING;
		vx = WING_GOOMBA_SPEED_WALKING_WHEN_FLYING;	// Giảm vận tốc vx để tạo cảm giác có lực cản không khí trên cao nên không thể di chuyển theo trục x nhanh được
		ay = WING_GOOMBA_GRAVITY_WHEN_FLYING;	// Giảm gia tốc ay để tạo cảm giác hạ cánh từ từ
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
	left = x - WING_GOOMBA_BBOX_WIDTH / 2;
	top = y - WING_GOOMBA_BBOX_HEIGHT / 2;
	right = left + WING_GOOMBA_BBOX_WIDTH;
	bottom = top + WING_GOOMBA_BBOX_HEIGHT;
}