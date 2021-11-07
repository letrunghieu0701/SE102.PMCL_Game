#include "Mushroom.h"

void CMushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MUSHROOM_BBOX_WIDTH / 2;
	top = y - MUSHROOM_BBOX_HEIGHT / 2;
	right = left + MUSHROOM_BBOX_WIDTH;
	bottom = top + MUSHROOM_BBOX_HEIGHT;
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;

	// Kiểm tra xem mushroom đã hết overlap với ? brick hay chưa
	// để có thể làm cho mushroom khỏi trồi lên nữa, sau đó sẽ cho mushroom di chuyển
	if ((y + DISTANCE_ADD_GRAVITY) < default_pos_y)
	{
		SetState(MUSHROOM_STATE_MOVING);
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
	//DebugOutTitle(L"Mushroom: vx: %0.2f vy: %0.2f ax: %0.2f ay: %0.2f", vx, vy, ax, ay);
	DebugOutTitle(L"State: %d", state);
}

void CMushroom::Render()
{
	CAnimations* animations = CAnimations::GetInstance();

	int ani_id = ID_ANI_MUSHROOM;

	animations->Get(ani_id)->Render(x, y);

	RenderBoundingBox();
}

void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CMushroom::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
		case MUSHROOM_STATE_IDLE:
			ax = 0;
			ay = 0;
			break;
		case MUSHROOM_STATE_RISING:
			vy = -MUSHROOM_RISING_UP_SPEED;
			break;
		case MUSHROOM_STATE_MOVING:
			//CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

			vx = MUSHROOM_WALKING_SPEED;
			ay = MUSHROOM_GRAVITY;
			break;
	}
}
