#include "Crumb.h"


void CCrumb::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;
	y += vy * dt;
}

void CCrumb::Render()
{
	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);
	float width = right - left;
	float height = bottom - top;

	CAnimations::GetInstance()->Get(ID_ANI_CRUMB)->Render(x + width / 2, y + height / 2);
	RenderBoundingBox();
}

void CCrumb::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + CRUMB_WIDTH - 1;
	bottom = top + CRUMB_HEIGHT - 1;
}