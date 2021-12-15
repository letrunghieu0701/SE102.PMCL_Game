#include "Tail.h"

void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Nếu cái đuôi hết thời gian tồn tại thì xóa nó đi
	if (this->StillHaveLivingTimeLeft() == false)
	{
		this->Delete();
	}
}

bool CTail::StillHaveLivingTimeLeft()
{
	ULONGLONG time_passed = GetTickCount64() -  this->living_start;

	if (0 <= time_passed && time_passed <= TAIL_LIFE_TIME)
		return true;

	return false;
}

void CTail::Render()
{
	RenderBoundingBox();
}

void CTail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + this->width - 1;
	bottom = top + this->height - 1;
}

void CTail::RenderBoundingBox(void)
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l + 1;
	rect.bottom = (int)b - (int)t + 1;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	CGame::GetInstance()->Draw(x + rect.right / 2 - cx, y + rect.bottom / 2 - cy, bbox, nullptr, BBOX_ALPHA, rect.right, rect.bottom);
}
