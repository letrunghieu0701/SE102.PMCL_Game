#pragma once
#include "GameObject.h"

// Physical stat
#define KOOPA_SPEED_GRAVITY 0.002f
#define KOOPA_SPEED_WALKING 0.05f
#define KOOPA_SPEED_SPINNING 0.1f

#define KOOPA_SPEED_BOUNCE_X 0.05f
#define KOOPA_SPEED_BOUNCE_Y 0.2f

#define KOOPA_SPEED_JUMP_DEFLECT 0.02f


#define KOOPA_FALL_DOWN_SPEED_Y		0.2f

// Extra settings
#define MAX_DISTANCE_ON_Y_BETWEEN_KOOPA_CDOP 25
#define KOOPA_SHELL_FACING_DOWN 0
#define KOOPA_SHELL_FACING_UP	1

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
#define KOOPA_STATE_SHELL_BOUNCE_UP 4

// Animation ID
#define ID_ANI_KOOPA_WALKING_LEFT		6000
#define ID_ANI_KOOPA_WALKING_RIGHT		6001

#define ID_ANI_KOOPA_SPIN_SHELL_DOWN_LEFT	6010
#define ID_ANI_KOOPA_SPIN_SHELL_DOWN_RIGHT	6011

#define ID_ANI_KOOPA_SPIN_SHELL_UP_LEFT		6015
#define ID_ANI_KOOPA_SPIN_SHELL_UP_RIGHT	6016

#define ID_ANI_KOOPA_SHELL_DOWN			6020
#define ID_ANI_KOOPA_SHELL_UP			6021


//Điều chỉnh độ cao khi chui ra khỏi mai rùa
#define KOOPA_SHELL_2_WALK_HEIGHT_ADJUST (KOOPA_BBOX_WALKING_HEIGHT - KOOPA_BBOX_SHELL_HEIGHT) + 1


// Time
#define KOOPA_TIME_SHELLING 5000
#define KOOPA_TIME_BOUNCE 500

class CKoopa: public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG shell_start;

	int id_CDOP; // Có thể quay đầu khi đi trên platform trên không trung hay không

	int current_state;

	ULONGLONG bounce_start;
	int hit_direction;
	bool hit_by_mario;

	int shell_direction;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithWingGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);

	int GetAniIdWalk();
	int GetAniIdSpinShell();
	int GetAniIdShell();

public:
	CKoopa(float x, float y, int type, bool can_turn = false, int id_CDOP = -1);
	virtual void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	ULONGLONG IsCanInShell()
	{ 
		ULONGLONG time_passed = GetTickCount64() - this->shell_start;
		return (0 <= time_passed &&
			time_passed <= KOOPA_TIME_SHELLING);
	}
	void SetAccel(float ax, float ay) { this->ax = ax; this->ay; }

	void StartBounce(int hit_direc)
	{
		this->bounce_start = GetTickCount64();
		this->hit_direction = hit_direc; 
		this->hit_by_mario = true;
		this->SetState(KOOPA_STATE_SHELL_BOUNCE_UP);
	}
	bool IsInBounceTime()
	{
		ULONGLONG time_passed = GetTickCount64() - bounce_start;
		return (0 <= time_passed && time_passed <= KOOPA_TIME_BOUNCE);
	}

	void SetShellDirection(int direction) { this->shell_direction = direction; }
};

