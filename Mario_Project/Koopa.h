#pragma once
#include "GameObject.h"

#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 27

#define ID_ANI_KOOPA_WALKING_LEFT 6000


class CKoopa: public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG shell_start; 

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CKoopa(float x, float y, int type);
	virtual void SetState(int state);
};

