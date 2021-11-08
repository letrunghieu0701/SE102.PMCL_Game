#pragma once
#include "Goomba.h"

#define WING_GOOMBA_GRAVITY 0.002f
#define WING_GOOMBA_SPEED_WALKING	0.05f

#define WING_GOOMBA_BBOX_WIDTH 20
#define WING_GOOMBA_BBOX_HEIGHT 19
#define WING_GOOMBA_BBOX_HEIGHT_DIE 9

#define WING_GOOMBA_DIE_TIMEOUT 500

#define ID_ANI_WING_GOOMBA_WALKING 5500
#define ID_ANI_WING_GOOMBA_FLYING 5510
#define ID_ANI_WING_GOOMBA_DIE 5520

#define WING_GOOMBA_STATE_DIE 0
#define WING_GOOMBA_STATE_WALKING 1
#define WING_GOOMBA_STATE_FLYING 2

class CWingGoomba: public CGoomba
{
private:
	float walkingDistance;
	float startWalkingLocation;

protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CWingGoomba(float x, float y, int type, float walking_distance);
	virtual void SetState(int state);
};

