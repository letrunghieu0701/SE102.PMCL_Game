#pragma once
#include "GameObject.h"
#include "Goomba.h"

#include "Textures.h"

#include "Collision.h"

// Physical Stats
#define TAIL_SPEED_X 0.2f

// Distance
#define TAIL_DISTANCE_MARIO_HEAD2TAIL 18
// Width và Height nhiều hơn sprite thực tế của tail là để tránh việc quá chú trọng vào từng pixel
// Player sẽ vui hơn nếu hitbox lớn hơn so với thực tế, tạo cảm giác dễ dàng đánh trúng enemy hơn
#define TAIL_WIDTH	12
#define TAIL_HEIGHT 8

// Time
#define TAIL_LIFE_TIME 500

class CTail : public CGameObject
{
protected:
	float width;
	float height;

	int slide_direction; // Hướng để lần đầu trượt cái đuôi qua, lần trượt tiếp theo sẽ là ngược lại hướng này

	bool turn_back;

	float max_travel_distance_left;
	float max_travel_distance_right;

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);

public:
	CTail(float x, float y, int direction, int mario_width, int type) :CGameObject(x, y, type)
	{
		this->width = TAIL_WIDTH;
		this->height = TAIL_HEIGHT;

		this->turn_back = false;
		this->slide_direction = direction;
		this->vx = this->slide_direction * TAIL_SPEED_X;

		this->max_travel_distance_left = x - this->width;
		this->max_travel_distance_right = x + (float)mario_width;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	void RenderBoundingBox(void);
	int IsBlocking() { return 0; }
	int IsCollidable() { return 1; }
};

