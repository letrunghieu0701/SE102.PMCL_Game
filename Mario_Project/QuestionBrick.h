#pragma once
#include <unordered_map>

#include "Animation.h"
#include "Animations.h"
#include "debug.h"

#include "GameObject.h"
#include "Brick.h"
#include "Mushroom.h"


#define QUESTION_BRICK_BOUNCING_UP_SPEED 0.15f
#define QUESTION_BRICK_GRAVITY 0.002f

#define ID_ANI_QUESTION_BRICK_QUESTION_MARK_MOVING 10100
#define ID_ANI_QUESTION_BRICK_NO_QUESTION_MARK_MOVING 10150
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

	int itemInsideId;
public:
	CQuestionBrick(float x, float y, int type, int item_id = -1) : CBrick(x, y, type)
	{
		default_pos_y = y;
		SetState(QUESTION_BRICK_STATE_IDLE_HAVE_MUSHROOM);

		itemInsideId = item_id;
	}

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	void SetState(int state);
};

