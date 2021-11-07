#pragma once
#pragma once
#include "GameObject.h"

#define MUSHROOM_RISING_UP_SPEED 0.01f
#define MUSHROOM_GRAVITY 0.002f
#define MUSHROOM_WALKING_SPEED 0.05f

#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

#define ID_ANI_MUSHROOM 11000

#define MUSHROOM_STATE_IDLE 0
#define MUSHROOM_STATE_RISING 1
#define MUSHROOM_STATE_MOVING 2

class CMushroom: public CGameObject
{
protected:
	float ax, ay;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CMushroom(int x, int y, int type) : CGameObject(x, y, type)
	{
		ax = 0;
		ay = 0;
		SetState(MUSHROOM_STATE_MOVING);
	}
	virtual void SetState(int state);
};

