#pragma once

#include "Game.h"
#include "GameObject.h"
#include "Mario.h"

#define MUSHROOM_RISING_UP_SPEED 0.01f
#define MUSHROOM_GRAVITY 0.002f
#define MUSHROOM_WALKING_SPEED 0.05f

#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16
#define DISTANCE_ADD_GRAVITY MUSHROOM_BBOX_HEIGHT + 1

#define ID_ANI_MUSHROOM 11000

#define MUSHROOM_STATE_IDLE 0
#define MUSHROOM_STATE_RISING 1
#define MUSHROOM_STATE_MOVING 2

//class CPlayScene;
//typedef CPlayScene* LPPLAYSCENE;

class CMushroom: public CGameObject
{
protected:
	float ax, ay;
	float default_pos_y;	// Để kiểm tra xem Mushroom đã trồi qua khỏi ? brick, rồi sau đó sẽ gán trọng lực cho ? brick

	int id;		// ID để có thể liên kết giữa ? brick và mushroom

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CMushroom(int x, int y, int type, int mushroom_id) : CGameObject(x, y, type)
	{
		ax = 0;
		ay = 0;
		default_pos_y = y;
		id = mushroom_id;
		SetState(MUSHROOM_STATE_IDLE);
	}
	virtual void SetState(int state);
};

