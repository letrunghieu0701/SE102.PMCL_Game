#pragma once
#include "GameObject.h"

// Bounding Box
#define CDOP_BBOX_WIDTH 16
#define CDOP_BBOX_HEIGHT 16

// Animation ID
#define ID_ANI_CDOP	7000


// Gọi class này là CDOP cho ngắn
class CChangeDirectionOnPlatform: public CGameObject
{
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	CChangeDirectionOnPlatform(float x, float y, int type);
	virtual void SetState(int state);
};

