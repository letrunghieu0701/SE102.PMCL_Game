#pragma once
#include "Brick.h"

#define BREAKABLE_BRICK_BBOX_WIDTH 16
#define BREAKABLE_BRICK_BBOX_HEIGHT 16

#define ID_ANI_BREAKABLE_BRICK	10200

class CBreakableBrick: public CBrick
{
public:
	CBreakableBrick(float x, float y, int type) :CBrick(x, y, type)
	{
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void virtual SetState(int state);
};

