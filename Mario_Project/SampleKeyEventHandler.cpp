#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "Tail.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();


	switch (KeyCode)
	{
	case DIK_W:
	{
		mario->SetHoldKoopaButton(true);
		//DebugOut(L"Đang bấm phím mũi cầm Koopa\n");
		break;
	}
	case DIK_DOWN:
	{
		mario->SetState(MARIO_STATE_SIT);
		break;
	}
	case DIK_S:
	{
		if (mario->GetLevel() == MARIO_LEVEL_RACCON)
		{
			// Mario ko thể tấn công bằng đuôi khi đang bay hoặc đang cầm Koopa
			if (!mario->CanContinueFly() && !mario->IsHolding())
			{
				int attack_direction = mario->GetNormalDirectionX();

				float mario_x, mario_y, mario_width;
				mario->GetPosition(mario_x, mario_y);
				mario->GetWidth(mario_width);

				CTail* mario_tail = new CTail(mario_x, mario_y + TAIL_DISTANCE_MARIO_HEAD2TAIL, -attack_direction, mario_width, OBJECT_TYPE_TAIL);
				LPPLAYSCENE play_scene = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene());
				play_scene->AddGameObject(mario_tail);
				mario->AttackWithTail();
			}
		}
		break;
	}

	case DIK_SPACE:
		float vx, vy;
		mario->GetSpeed(vx, vy);

		if (mario->GetLevel() == MARIO_LEVEL_RACCON &&
			abs(vx) == MARIO_RUNNING_SPEED &&
			mario->IsOnPlatform() == false &&
			mario->GetFlyingStart() == 0)
		{
			mario->SetState(MARIO_STATE_FLYING);
			mario->StartCanFly(GetTickCount64());

		}
		else if (mario->CanContinueFly())
		{
			mario->SetState(MARIO_STATE_FLYING);
		}
		else
			mario->SetState(MARIO_STATE_JUMP);
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_RACCON);
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	case DIK_R: // reset
		//Reload();
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (KeyCode)
	{
	case DIK_SPACE:
		mario->SetState(MARIO_STATE_RELEASE_JUMP);
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	case DIK_W:
		mario->SetHoldKoopaButton(false);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE* states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	mario->IsPressingUpButton(false);

	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else if (game->IsKeyDown(DIK_UP))
	{
		mario->IsPressingUpButton(true);
		//DebugOut(L"Đang bấm phím mũi tên UP\n");
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		if (mario->IsOnPipeGate())
		{
			float mario_left, mario_top, mario_right, mario_bottom;
			mario->GetBoundingBox(mario_left, mario_top, mario_right, mario_bottom);

			float pipegate_left, pipegate_top, pipegate_right, pipegate_bottom;
			mario->GetCurrentPipeGate()->GetBoundingBox(pipegate_left, pipegate_top, pipegate_right, pipegate_bottom);

			// Mario phải đứng ở giữa pipegate thì mới cho chui vào
			if (pipegate_left <= mario_left && mario_right <= pipegate_right)
			{
				mario->ReadyToGoIntoPipeGate(true);
				//DebugOut(L"Sẵn sàng vào Pipe Gate In\n");
			}
		}
	}
	else
		mario->SetState(MARIO_STATE_IDLE);
}