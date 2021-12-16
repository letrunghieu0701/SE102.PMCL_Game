#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define CRUMB_SPEED_X 0.07f
#define CRUMB_SPEED_Y 0.1f

#define CRUMG_SHOOT_FORCE_STRONG_Y	0.15f
#define CRUMG_SHOOT_FORCE_WEAK_Y	0.07f

#define CRUMB_GRAVITY 0.002f

#define CRUMB_SHOOT_LEFT -1
#define CRUMB_SHOOT_RIGHT 1

#define CRUMB_WIDTH 8
#define CRUMB_HEIGHT 8

#define ID_ANI_CRUMB 15000

#define CRUMB_LIVE_TIME 500

class CCrumb: public CGameObject
{
private:
	ULONGLONG live_start;
	float ay;
public:
	CCrumb(float x, float y, int type, ULONGLONG time_live_start, float vy, float nx) :CGameObject(x, y, type)
	{
		this->live_start = time_live_start;

		if (nx > 0)
			this->vx = CRUMB_SPEED_X;
		else
			this->vx = -CRUMB_SPEED_X;
		this->vy = -vy;

		this->ay = CRUMB_GRAVITY;
	};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsBlocking() { return 0; }

	bool IsInLivingTime()
	{
		ULONGLONG time_passed = GetTickCount64() - live_start;
		return (0 <= time_passed && time_passed <= CRUMB_LIVE_TIME);
	}
};

