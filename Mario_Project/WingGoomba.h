#pragma once
#include "Goomba.h"

#define WING_GOOMBA_GRAVITY 0.002f
#define WING_GOOMBA_GRAVITY_WHEN_FLYING 0.0002f
#define WING_GOOMBA_SPEED_WALKING_WHEN_FLYING 0.03f
#define WING_GOOMBA_SPEED_WALKING 0.05f
#define WING_GOOMBA_SPEED_FLYING 0.1f

#define WING_GOOMBA_BBOX_WIDTH 20
#define WING_GOOMBA_BBOX_HEIGHT 19
#define WING_GOOMBA_BBOX_HEIGHT_DIE 9

#define WING_GOOMBA_DIE_TIMEOUT 500

#define ID_ANI_WING_GOOMBA_HAVE_WING_WALKING 5500
#define ID_ANI_WING_GOOMBA_HAVE_WING_FLYING 5510
#define ID_ANI_WING_GOOMBA_NO_WING_WALKING 5520
#define ID_ANI_WING_GOOMBA_DIE 5530

#define WING_GOOMBA_STATE_DIE 0
#define WING_GOOMBA_STATE_WALKING 1
#define WING_GOOMBA_STATE_FLYING 2

#define WING_GOOMBA_LEVEL_HAVE_WING 0
#define WING_GOOMBA_LEVEL_NO_WING 1

class CWingGoomba: public CGoomba
{
private:
	int level;
	float walkingDistance;
	float startWalkingLocation;

	int GetAniIdHaveWing();
	int GetAniIdNoWing();

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

	void SetLevel(int l);
	int GetLevel();
};

