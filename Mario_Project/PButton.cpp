#include "PButton.h"

CPButton::CPButton(float x, float y, int type) :CGameObject(x, y, type)
{
	state = PBUTTON_STATE_NORMAL;
}


void CPButton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == PBUTTON_STATE_PRESSED)
	{
		LPPLAYSCENE play_scene = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene());

		vector<LPGAMEOBJECT> game_objects;
		play_scene->GetGameObjects(game_objects);

		for (size_t i = 0; i < game_objects.size(); i++)
		{
			if (game_objects[i]->GetType() == OBJECT_TYPE_BREAKABLE_BRICK)
			{
				float x, y;
				game_objects[i]->GetPosition(x, y);
				CCoin* coin = new CCoin(x, y, OBJECT_TYPE_COIN, true);
				coin->StartLiving();

				play_scene->AddGameObject(coin);
				
				game_objects[i]->Delete();
			}
		}

		state = PBUTTON_STATE_CANNOT_PRESS;
	}
}

void CPButton::Render()
{
	CAnimations* animations = CAnimations::GetInstance();

	int ani_id = ID_ANI_PBUTTON_BLUE;
	if (state == PBUTTON_STATE_NORMAL)
		ani_id = ID_ANI_PBUTTON_BLUE;
	else if (state == PBUTTON_STATE_PRESSED || state == PBUTTON_STATE_CANNOT_PRESS)
		ani_id = ID_ANI_PBUTTON_PRESSED;

	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);
	float width = right - left;
	float height = bottom - top;

	CAnimations::GetInstance()->Get(ani_id)->Render(x + width / 2, y + height / 2);

	//RenderBoundingBox();
	DebugOutTitle(L"P button x: %0.2f, y: %0.2f", x, y);
}

void CPButton::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == PBUTTON_STATE_NORMAL)
	{
		left = x;
		top = y;
		right = left + PBUTTON_BBOX_WIDTH - 1;
		bottom = top + PBUTTON_BBOX_HEIGHT - 1;
	}
	else if (state == PBUTTON_STATE_PRESSED || state == PBUTTON_STATE_CANNOT_PRESS)
	{
		left = x;
		top = y;
		right = left + PBUTTON_PRESSED_BBOX_WIDTH - 1;
		bottom = top + PBUTTON_PRESSED_BBOX_HEIGHT - 1;
	}

}

void CPButton::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case PBUTTON_STATE_PRESSED:
		y += PBUTTON_PUSH_DOWN_DISTANCE;
		break;
	}
}
