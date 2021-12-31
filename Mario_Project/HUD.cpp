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
	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);
	float width = right - left;
	float height = bottom - top + 1;

	CAnimations::GetInstance()->Get(ID_ANI_HUD)->Render(x + width / 2, y + height / 2);
	//RenderBoundingBox();
}


void CHUD::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + HUD_BBOX_WIDTH - 1;
	bottom = top + HUD_BBOX_HEIGHT - 1;
}

void CHUD::SetState(int state)
{
}
