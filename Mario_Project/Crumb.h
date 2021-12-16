#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define CRUMB_SPEED_X 0.05f
#define CRUMB_SPEED_Y 0.05f

#define CRUMB_WIDTH 8
#define CRUMB_HEIGHT 8

#define ID_ANI_CRUMB 15000

class CCrumb: public CGameObject
{
private:
	ULONGLONG live_start;
public:
	CCrumb(float x, float y, int type, ULONGLONG time_live_start) :CGameObject(x, y, type)
	{
		this->live_start = time_live_start;
		vx = CRUMB_SPEED_X;
		vy = CRUMB_SPEED_Y;
	};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsBlocking() { return 0; }
};

