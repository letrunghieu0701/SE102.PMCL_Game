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
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
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
			ay = MUSHROOM_GRAVITY;
			break;
	}
}
