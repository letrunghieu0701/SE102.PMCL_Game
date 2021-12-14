#pragma once
#include "GameObject.h"

// Physical stat
#define KOOPA_SPEED_GRAVITY 0.002f
#define KOOPA_SPEED_WALKING 0.05f
#define KOOPA_SPEED_SPINNING 0.1f


#define KOOPA_FALL_DOWN_SPEED_Y		0.2f

// Extra settings
#define MAX_DISTANCE_ON_Y_BETWEEN_KOOPA_CDOP 20

// Bounding Box
#define KOOPA_BBOX_WALKING_WIDTH 16
#define KOOPA_BBOX_WALKING_HEIGHT 27
#define KOOPA_BBOX_SHELL_WIDTH 16
#define KOOPA_BBOX_SHELL_HEIGHT 16

// State
#define KOOPA_STATE_WALKING 0
#define	KOOPA_STATE_SHELLING 1
#define KOOPA_STATE_SPIN_SHELL 2
#define KOOPA_STATE_HOLDED_BY_MARIO 3

// Animation ID
#define ID_ANI_KOOPA_WALKING_LEFT 6000
#define ID_ANI_KOOPA_WALKING_RIGHT 6001
#define ID_ANI_KOOPA_SHELLING 6002
#define ID_ANI_KOOPA_SPIN_SHELL_LEFT 6003
#define ID_ANI_KOOPA_SPIN_SHELL_RIGHT 6004


//Điều chỉnh độ cao khi chui ra khỏi mai rùa
#define KOOPA_SHELL_2_WALK_HEIGHT_ADJUST (KOOPA_BBOX_WALKING_HEIGHT - KOOPA_BBOX_SHELL_HEIGHT)


// Time
#define KOOPA_TIME_SHELLING 5000


class CKoopa: public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG shell_start;

	int id_CDOP;

	int current_state;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithWingGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCDOP(LPCOLLISIONEVENT e);

	int GetAniIdWalk();
	int GetAniIdSpinShell();

public:
	CKoopa(float x, float y, int type, int id_CDOP);
	virtual void SetState(int state);

	ULONGLONG IsInShell()
	{ 
		ULONGLONG time_passed = GetTickCount64() - this->shell_start;
		return (0 <= time_passed &&
			time_passed <= KOOPA_TIME_SHELLING);
	}
	void SetAccel(float ax, float ay) { this->ax = ax; this->ay; }
};

