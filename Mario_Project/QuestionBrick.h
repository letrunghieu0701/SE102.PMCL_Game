#pragma once
#include "GameObject.h"
#include "Brick.h"
#include "Animation.h"
#include "Animations.h"
#include "debug.h"

#define QUESTION_BRICK_BOUNCING_UP_SPEED 0.3f
#define QUESTION_BRICK_GRAVITY 0.002f

#define ID_ANI_QUESTION_BRICK 10500
#define QUESTION_BRICK_WIDTH 16
#define QUESTION_BRICK_BBOX_WIDTH 16
#define QUESTION_BRICK_BBOX_HEIGHT 16

#define QUESTION_BRICK_STATE_IDLE_HAVE_MUSHROOM 0
#define	QUESTION_BRICK_STATE_BOUNCING_UP 1
#define QUESTION_BRICK_STATE_IDLE_NO_MUSHROOM 2

class CQuestionBrick : public CBrick
{
private:
	float ay;

	float default_pos_y;
public:
	CQuestionBrick(float x, float y, int type) : CBrick(x, y, type)
	{
		default_pos_y = y;
		SetState(QUESTION_BRICK_STATE_IDLE_HAVE_MUSHROOM);
	}

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	void SetState(int state);
};

