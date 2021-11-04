#pragma once
#include "GameObject.h"
#include "Brick.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_QUESTION_BRICK 10500
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

class CQuestionBrick: public CBrick
{
public:
	CQuestionBrick(float x, float y, int type) : CBrick(x, y, type) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

