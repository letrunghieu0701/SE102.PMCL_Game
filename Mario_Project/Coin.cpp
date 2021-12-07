#include "Coin.h"
#include "PlayScene.h"


void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->usedToBeBreakableBrick)
	{
		if (GetTickCount64() - this->lifeTime_start > COIN_2_BBRICK_TIME/*this->lifeTimeBecomeBreakableBrick*/)
		{
			this->Delete();

			CBreakableBrick* bbrick = new CBreakableBrick(this->x, this->y, OBJECT_TYPE_BREAKABLE_BRICK, this->lifeTimeBecomeBreakableBrick);
			LPPLAYSCENE play_scene = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene());
			play_scene->AddBreakableBricks(bbrick);
			play_scene->AddGameObject(bbrick);

			int a = play_scene->GetBBricksSize();
			DebugOut(L"size: %d \n", a);
		}
	}
}

void CCoin::Render()
{
	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);
	float width = right - left;
	float height = bottom - top;

	CAnimations::GetInstance()->Get(ID_ANI_COIN)->Render(x + width / 2, y + height / 2);
	//CAnimations::GetInstance()->Get(ID_ANI_COIN)->Render(x, y);

	RenderBoundingBox();
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}