#pragma once
#include "GameObject.h"

// Physical stat
#define KOOPA_SPEED_GRAVITY 0.002f
#define KOOPA_SPEED_WALKING 0.05f
#define KOOPA_SPEED_SPINNING 0.3f

// Bounding Box
#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 27

// State
#define KOOPA_STATE_WALKING 0
#define	KOOPA_STATE_SHELLING 1
#define KOOPA_STATE_SPIN_SHELL 2

// Animation ID
#define ID_ANI_KOOPA_WALKING_LEFT 6000
#define ID_ANI_KOOPA_WALKING_RIGHT 6001
#define ID_ANI_KOOPA_SHELLING 6002
#define ID_ANI_KOOPA_SPIN_SHELL_LEFT 6003
#define ID_ANI_KOOPA_SPIN_SHELL_RIGHT 6004


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
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithWingGoomba(LPCOLLISIONEVENT e);

	int GetAniIdWalk();
	int GetAniIdSpinShell();

public:
	CKoopa(float x, float y, int type);
	virtual void SetState(int state);
};

