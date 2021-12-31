#include "HUD.h"

void CHUD::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Cập nhật vị trí cho HUD
	CGame* g = CGame::GetInstance();
	g->GetCamPos(x, y);

	y += g->GetBackBufferHeight() - HUD_BBOX_HEIGHT;
}

void CHUD::Render()
{
	// Vẽ hud
	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);
	float width = right - left;
	float height = bottom - top + 1;

	CSprites* s = CSprites::GetInstance();
	s->Get(ID_SPRITE_HUD + 1)->Draw(x + width / 2, y + height / 2);


	// Vẽ các mũi tên vận tốc
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	float speed_per_arrow = (float)((MARIO_RUNNING_SPEED - MARIO_WALKING_SPEED) / NUM_SPEED_ARROW);
	for (int arrow_ith = 1; arrow_ith <= NUM_SPEED_ARROW; arrow_ith++)
	{
		int sprite_id = ID_SPRITE_SPEED_ARROW_BLACK;

		float mario_vx;
		mario->GetSpeedX(mario_vx);
		float mario_diff_current_speed_to_walk_speed = abs(mario_vx) - MARIO_WALKING_SPEED;

		// Nếu đang bay hoặc max speed thì đều tăng thanh tốc độ
		if (mario->CanContinueFly() ||
			(mario_diff_current_speed_to_walk_speed >= speed_per_arrow * arrow_ith))
			sprite_id = ID_SPRITE_SPEED_ARROW_WHITE + 1;
		else
			sprite_id = ID_SPRITE_SPEED_ARROW_BLACK + 1;

		s->Get(sprite_id)->Draw(x + SPEED_ARROW_POSITION_RELATE_2_HUD_X + SPEED_ARROW_WIDTH / 2 + arrow_ith*SPEED_ARROW_WIDTH,
			y + SPEED_ARROW_POSITION_RELATE_2_HUD_Y + SPEED_ARROW_HEIGHT / 2);
	}
	

	//RenderBoundingBox();
}


void CHUD::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + HUD_BBOX_WIDTH - 1;
	bottom = top + HUD_BBOX_HEIGHT - 1;
}
