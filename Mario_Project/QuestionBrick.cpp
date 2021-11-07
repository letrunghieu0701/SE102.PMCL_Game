#include "QuestionBrick.h"


void CQuestionBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_QUESTION_BRICK)->Render(x, y);
	RenderBoundingBox();
}

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;

	y += vy * dt;

	if (y > default_pos_y)
	{
		ay = 0;
		vy = 0;

		y = default_pos_y;
	}

	DebugOutTitle(L"? Brick: y: %0.2f  vy: %0.2f  ay: %0.2f", y, vy, ay);
}

void CQuestionBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - QUESTION_BRICK_BBOX_WIDTH / 2;
	t = y - QUESTION_BRICK_BBOX_HEIGHT / 2;
	r = l + QUESTION_BRICK_BBOX_WIDTH;
	b = t + QUESTION_BRICK_BBOX_HEIGHT;
}

void CQuestionBrick::SetState(int state)
{
	CBrick::SetState(state);

	switch (state)
	{
	case QUESTION_BRICK_STATE_IDLE_HAVE_MUSHROOM:
		vy = 0;
		ay = 0;
		break;
	case QUESTION_BRICK_STATE_BOUNCING_UP:
		vy = -QUESTION_BRICK_BOUNCING_UP_SPEED;
		ay = QUESTION_BRICK_GRAVITY;

		ay = QUESTION_BRICK_GRAVITY;
		break;
	}
	
		
}
