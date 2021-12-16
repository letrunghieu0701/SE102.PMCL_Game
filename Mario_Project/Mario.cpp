#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"
#include "QuestionBrick.h"
#include "WingGoomba.h"


#include "Collision.h"
#include "PlayScene.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;
	if (vy > MARIO_FALL_DOWN_SPEED_Y) vy = MARIO_FALL_DOWN_SPEED_Y;



	if (this->isGettingOutOfPipeDesOut)
	{
		this->vy = -MARIO_FALL_DOWN_2_PIPEGATE_SPEED_Y;
		this->vx = 0;

		float left, top, right, bottom;
		GetBoundingBox(left, top, right, bottom);
		float current_height = bottom - top;

		DebugOutTitle(L"Mario_y = %0.2f PipeDes_y = %0.2f", y, this->current_pipedes->GetPosY());

		// Nếu đã chui ra xong
		// thì trả lại quyền điều khiển Mario cho player
		if (this->current_pipedes->GetPosY() - this->y > current_height)
		{
			this->isGettingOutOfPipeDesOut = false;
			this->current_pipedes = nullptr;
		}
	}

	if (this->isGoingIntoPipeGate)
	{
		// Đang chui vào lối vào của Hidden Zone
		if (this->current_pipegate->IsGateIn())
		{
			this->vy = MARIO_FALL_DOWN_2_PIPEGATE_SPEED_Y;
			this->vx = 0;

			if (this->y > this->current_pipegate->GetPosY())
			{
				this->isGoingIntoPipeGate = false;

				int pipe_des_id = this->current_pipegate->GetPipeDesID();
				unordered_map<int, LPGAMEOBJECT> item_list = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetItemList();
				CPipeTeleportDestination* pipe_des = dynamic_cast<CPipeTeleportDestination*>(item_list[pipe_des_id]);
				float pipeDes_x, pipeDes_y;
				pipe_des->GetPosition(pipeDes_x, pipeDes_y);

				this->current_pipegate = nullptr;

				// Đẩy sang ngang để tránh Mario đứng trên các brick ở bức tường bên trái trong Hidden Zone
				this->SetPosition(pipeDes_x + MARIO_PUSH_HORIZONTAL, pipeDes_y);
			}
		}
		// Đang chui vào lối ra của Hidden Zone
		else
		{
			this->vy = -MARIO_FALL_DOWN_2_PIPEGATE_SPEED_Y;
			this->vx = 0;

			float left, top, right, bottom;
			GetBoundingBox(left, top, right, bottom);
			float current_height = bottom - top;

			if (this->oldPos_y - this->y >= current_height)
			{
				this->isGoingIntoPipeGate = false;

				int pipe_des_id = this->current_pipegate->GetPipeDesID();
				unordered_map<int, LPGAMEOBJECT> item_list = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetItemList();
				CPipeTeleportDestination* pipe_des = dynamic_cast<CPipeTeleportDestination*>(item_list[pipe_des_id]);
				float pipeDes_x, pipeDes_y;
				pipe_des->GetPosition(pipeDes_x, pipeDes_y);

				this->current_pipegate = nullptr;

				// Đánh dấu pipe_des để có thể so sánh vị trí khi đang chui ra khỏi pipe_des_out để về mặt đất
				this->current_pipedes = pipe_des;

				// Đẩy sang ngang để tránh Mario đứng trên các brick ở bức tường bên trái trong Hidden Zone
				this->SetPosition(pipeDes_x + MARIO_PUSH_HORIZONTAL, pipeDes_y);

				// Báo hiệu đang chui ra khỏi pipe_des_out để ra khỏi Hidden Zone
				this->isGettingOutOfPipeDesOut = true;
			}
		}
	}


	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	/*if (GetTickCount64() - attackTail_start > MARIO_ATTACK_TAIL_TIME)
		attackTail_start = 0;*/

		// Nếu vẫn còn trong thời gian rơi chậm
		// Thì giảm tốc độ vx và vy bằng tốc độ khi rơi chậm
	if ((0 <= (GetTickCount64() - fallSlow_start)) &&
		((GetTickCount64() - fallSlow_start) <= MARIO_FALL_SLOW_TIME))
	{
		if (nx > 0)
			maxVx = MARIO_SPEED_FALL_SLOW_X;
		else
			maxVx = -MARIO_SPEED_FALL_SLOW_X;
		vy = MARIO_SPEED_FALL_SLOW_Y;

		//DebugOut(L"Kich hoat fallSlow_start\n");
	}
	// Hoặc nếu thời gian rơi chậm đã hết
	// Thì không cho rơi chậm nữa
	else if ((GetTickCount64() - fallSlow_start) > MARIO_FALL_SLOW_TIME)
	{

		fallSlow_start = 0;

		//DebugOut(L"Tat fallSlow_start\n");
	}


	// Nếu vẫn đang trong thời gian bay
	// Thì sử dụng tốc độ di chuyển (theo trục x) đã được trừ đi lực cản không khí theo trục x
	if (this->CanContinueFly())
	{
		if (nx > 0)
			maxVx = MARIO_SPEED_FLYING_X;
		else
			maxVx = -MARIO_SPEED_FLYING_X;

		//DebugOut(L"Đang bay\n");
	}
	// Nếu không
	// Thì không cho khả năng bay khi nhấm phím nhảy nữa
	else
	{
		flying_start = 0;

		//DebugOut(L"Đang không bay\n");
	}
	//DebugOutTitle(L"Mario flying speed: vx = %0.2f vy = %0.2f max_vx = %0.2f", vx, vy, maxVx);


	this->isOnPlatform = false;
	this->isOnPipeGate = false;

	if (this->IsTurningOffCollision() == false)
	{
		this->turnOffCollision_start = 0;
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
	else
	{
		OnNoCollision(dt);
	}


	//DebugOutTitle(L"Mario speed: vx = %0.2f vy = %0.2f", vx, vy);

	// Xử lý login khi Mario cầm Koopa sau khi xử lý va chạm là vì:
	// Khi Mario cầm Koopa, mà Koopa thì thoát khỏi shell và đi bộ trở lại:
	// Cần xử lý va chạm để kéo Mario ra khỏi các blocking object trước, sau đó thì mới có thể xét lại vị trí y cho Koopa được
	// Vì logic hiện tại là dùng độ cao chênh lệch giữa Mario và Koopa để xét lại vị trí y cho Koopa khi Mario không thể cầm Koopa được nữa
	// Nên vị trí của Mario phải là vị trí đã được push back ra khỏi các blocking object, để có thể xét đúng được vị trí y cho Koopa
	if (this->isHoldingKoopa)
	{
		CKoopa* koopa = this->current_koopa_holding;

		// Nếu đã vượt quá thời gian trong mai rùa, thì:
		// 1. Phải thả Koopa ra
		// 2. Gây sát thương lên Mario (vì Mario cầm Koopa cho tới khi nó chui ra khỏi Shell luôn mà)
		if (koopa->IsCanInShell() == false)
		{
			// Điều chỉnh lại vị trí của Koopa để không bị overlap với Platform khi Koopa từ shell -> walk
			// Cho Koopa đi bộ
			// Hủy kết nối với Koopa

			// Phải set state của Koopa trước khi thay đổi vị trí y, vì hiện tại Koopa đang trong state shell
			// State shell có height thấp hơn state walking nhiều, và code ở đây cần chiều cao của state walking chứ không phải state shell
			koopa->SetState(KOOPA_STATE_WALKING);
			this->SetLevel(this->GetLevel() - 1);
			this->StartUntouchable();

			float mario_left, mario_top, mario_right, mario_bottom;
			this->GetBoundingBox(mario_left, mario_top, mario_right, mario_bottom);
			float mario_current_height = mario_bottom - mario_top;

			float koopa_left, koopa_top, koopa_right, koopa_bottom;
			koopa->GetBoundingBox(koopa_left, koopa_top, koopa_right, koopa_bottom);
			float koopa_height = koopa_bottom - koopa_top;

			float diff_koopa_vs_mario = 0;
			// Nếu Koopa mà cao hơn Mario, thì lấy vị trí y của Mario cộng thêm khoảng chênh lệch để làm vị trí y cho Koopa
			// Và cộng thêm một chút để tránh bị overlap với platform bên dưới
			if (koopa_height > mario_current_height)
			{
				diff_koopa_vs_mario = mario_current_height - (koopa_height - BLOCK_PUSH_FACTOR);
			}
			// Ngược lại, nếu Mario cao hơn Koopa thì dùng vị trí của Mario trừ đi khoảng chênh lệch để làm vị trí y cho Koopa
			else
			{
				diff_koopa_vs_mario = mario_current_height - koopa_height;
			}

			// To-do: cần xem xét lại, Koopa có thể overlap với Pipe, brick,... khi Mario cứ xét cứng vị trí của Koopa
			// mỗi khi Mario cầm Koopa
			// 
			// Dùng vị trí x của Koopa, vì Koopa không bị overlap theo trục x
			koopa->SetPosition(koopa_left, mario_top + diff_koopa_vs_mario);
			this->isHoldingKoopa = false;
			this->current_koopa_holding = nullptr;
		}
		else
		{
			// Nếu đang nhấn + giữ phím "cầm Koopa" thì kéo Koopa đi với Mario
			if (this->isPressingHoldKoopaButton)
			{
				CKoopa* koopa = this->current_koopa_holding;

				float left, top, right, bottom;
				this->GetBoundingBox(left, top, right, bottom);
				float current_height = bottom - top;
				float current_width = right - left;
				if (this->nx > 0)
				{
					koopa->SetPosition(this->x + current_width / 2, this->y + current_height / 4);
				}
				else
				{
					koopa->SetPosition(this->x - current_width / 2, this->y + current_height / 4);
				}

				koopa->SetNormalDirectionX(this->nx);

				float koopa_vx, koopa_vy;
				koopa->GetSpeed(koopa_vx, koopa_vy);
				DebugOut(L"Koopa speed: vx: %0.2f vy: %0.2f \n", koopa_vx, koopa_vy);

				//DebugOut(L"Đang set speed = 0 cho Koopa\n");
			}
			// Nếu đã thả phím "cầm Koopa" ra thì drop Koopa và đá Koopa xoay đi
			else
			{
				CKoopa* koopa = this->current_koopa_holding;
				koopa->SetNormalDirectionX(this->nx);
				koopa->SetState(KOOPA_STATE_SPIN_SHELL);

				this->isHoldingKoopa = false;
				this->current_koopa_holding = nullptr;
			}
		}

	}
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}


void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->IsBlocking())
	{
		if (e->ny != 0)
		{
			vy = 0;
			if (e->ny < 0)
			{
				isOnPlatform = true;

				if (e->obj->GetType() == OBJECT_TYPE_PIPE_GATE)
				{
					CPipeGate* pipe_gate = dynamic_cast<CPipeGate*>(e->obj);

					// Nếu là pipe gate In
					if (pipe_gate->IsGateIn())
					{
						this->isOnPipeGate = true;
						this->current_pipegate = dynamic_cast<CPipeGate*>(e->obj);
						DebugOut(L"Đang đứng trên Pipe Gate In\n");
					}
				}
			}
			else
			{
				if (e->obj->GetType() == OBJECT_TYPE_PIPE_GATE)
				{
					CPipeGate* pipe_gate = dynamic_cast<CPipeGate*>(e->obj);

					// Nếu là pipe gate để thoát khỏi Hidden Zone
					if (!pipe_gate->IsGateIn())
					{
						DebugOut(L"Đã chạm vào đáy của Pipe Gate Out\n");
					}
				}
			}
		}
		else if (e->nx != 0)
		{
			vx = 0;
		}
	}


	if (e->obj->GetType() == OBJECT_TYPE_GOOMBA)
		OnCollisionWithGoomba(e);
	else if (e->obj->GetType() == OBJECT_TYPE_WING_GOOMBA)
		OnCollisionWithWingGoomba(e);
	else if (e->obj->GetType() == OBJECT_TYPE_COIN)
		OnCollisionWithCoin(e);
	else if (e->obj->GetType() == OBJECT_TYPE_PORTAL)
		OnCollisionWithPortal(e);
	else if (e->obj->GetType() == OBJECT_TYPE_QUESTION_BRICK)
		OnCllisionWithQuestionBrick(e);
	else if (e->obj->GetType() == OBJECT_TYPE_MUSHROOM)
		OnCollisionWithMushroom(e);
	else if (e->obj->GetType() == OBJECT_TYPE_KOOPA)
		OnCollisionWithKoopa(e);
	else if (e->obj->GetType() == OBJECT_TYPE_PIPE_GATE)
		OnCollisionWithPipeGate(e);
	else if (e->obj->GetType() == OBJECT_TYPE_PIPE_TELEPORT_DESTINATION);
}

void CMario::OnCollisionWithPipeGate(LPCOLLISIONEVENT e)
{
	CPipeGate* pipe_gate = dynamic_cast<CPipeGate*>(e->obj);

	// Nếu đây là pipe để vào Hidden Zone
	if (pipe_gate->IsGateIn())
	{
		if (this->IsOnPipeGate() && this->CanGoIntoPipeGate())
		{
			float mario_left, mario_top, mario_right, mario_bottom;
			this->GetBoundingBox(mario_left, mario_top, mario_right, mario_bottom);

			float pipegate_left, pipegate_top, pipegate_right, pipegate_bottom;
			pipe_gate->GetBoundingBox(pipegate_left, pipegate_top, pipegate_right, pipegate_bottom);

			// Mario phải đứng ở giữa pipegate thì mới cho chui vào
			if (pipegate_left <= mario_left && mario_right <= pipegate_right)
			{
				this->StartTurnOffCollision();
				this->isGoingIntoPipeGate = true;
				this->current_pipegate = dynamic_cast<CPipeGate*>(e->obj);

				DebugOut(L"Tiến vào Pipe Gate vào Hidden Zone\n");
			}
		}
	}
	// Nếu đây là pipe để ra khỏi Hidden Zone
	else
	{
		if (e->ny == DIRECTION_DOWN && this->is_pressing_up_button)
		{
			float mario_left, mario_top, mario_right, mario_bottom;
			this->GetBoundingBox(mario_left, mario_top, mario_right, mario_bottom);

			float pipegate_left, pipegate_top, pipegate_right, pipegate_bottom;
			pipe_gate->GetBoundingBox(pipegate_left, pipegate_top, pipegate_right, pipegate_bottom);

			// Mario phải đứng ở giữa pipegate thì mới cho chui vào
			if (pipegate_left <= mario_left && mario_right <= pipegate_right)
			{
				this->StartTurnOffCollision();
				this->isGoingIntoPipeGate = true;
				this->oldPos_y = this->y;
				this->current_pipegate = dynamic_cast<CPipeGate*>(e->obj);

				DebugOut(L"Tiến vào Pipe Gate để ra khỏi Hidden Zone\n");
			}
		}
	}
}


void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);

	if (koopa->GetState() == KOOPA_STATE_WALKING)
	{
		if (e->ny < 0)	// Mario hit từ bên trên
		{
			koopa->SetState(KOOPA_STATE_SHELLING);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else  // Mario va chạm theo các hướng còn lại
		{
			if (untouchable == 0)
			{
				if (this->GetLevel() == MARIO_LEVEL_SMALL)
				{
					DebugOut(L">>> Mario DIE >>> \n");
					this->SetState(MARIO_STATE_DIE);
				}
				else if (this->GetLevel() == MARIO_LEVEL_BIG)
				{
					this->SetLevel(MARIO_LEVEL_SMALL);
					StartUntouchable();
				}
			}
		}
	}



	else if (koopa->GetState() == KOOPA_STATE_SHELLING)
	{
		if (this->isPressingHoldKoopaButton)
		{
			this->isHoldingKoopa = true;
			this->current_koopa_holding = koopa;
			koopa->SetState(KOOPA_STATE_HOLDED_BY_MARIO);
		}
		else
		{
			float mario_x, mario_y;
			float koopa_x, koopa_y;
			this->GetPosition(mario_x, mario_y);
			koopa->GetPosition(koopa_x, koopa_y);

			if (mario_x < koopa_x)	// Mario ở phía bên trái của Koopa
			{
				koopa->SetNormalDirectionX(DIRECTION_RIGHT);	// Cho xoay sang phải
				koopa->SetState(KOOPA_STATE_SPIN_SHELL);	// Đặt lại state để có chỉ số vật lý sau: vận tốc x có trị tuyệt đối rất lớn và cùng dấu (dương) với vectoc normal
			}
			else  // Mario ở phía bên phải của Koopa
			{
				koopa->SetNormalDirectionX(DIRECTION_LEFT);		// Cho xoay sang trái
				koopa->SetState(KOOPA_STATE_SPIN_SHELL);	// Đặt lại state để có chỉ số vật lý sau: vận tốc x có giá trị tuyệt đối rất lớn và cùng dấu (âm) với vector normal
			}
		}

	}



	else if (koopa->GetState() == KOOPA_STATE_SPIN_SHELL)
	{
		// Nếu Mario nhảy lên đầu Koopa khi Koopa đang xoay thì có thể khiến nó ngừng xoay
		if (e->ny == DIRECTION_UP)
		{
			koopa->SetState(KOOPA_STATE_SHELLING);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		// Mario va chạm với Koopa theo các hướng còn lại (trái, phải và bên trên của Mario)
		// Nếu Mario có thể bị đụng vào, thì mới xử lý va chạm. Còn nếu đang trong thời gian "không thể bị đụng vào" thì thôi, không làm gì cả
		else if (this->untouchable == 0)
		{
			// Nếu Mario có 2 mạng (level Big) thì giảm thành còn 1 mạng (level Small) rồi bắt đầu tính giờ cho untouchable
			if (this->GetLevel() == MARIO_LEVEL_BIG)
			{
				this->SetLevel(MARIO_LEVEL_SMALL);
				StartUntouchable();
			}
			// Còn nếu chỉ còn 1 mạng (level Small) thì cho Mario die luôn
			else if (this->GetLevel() == MARIO_LEVEL_SMALL)
			{
				DebugOut(L">>> Marioo DIE >>> \n");
				this->SetState(MARIO_STATE_DIE);
			}
		}
	}
}


void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	// Điều kiện kiểm tra state của Mushroom là để tránh Mario ăn được (va chạm) Mushroom trong khi Mushroom vẫn đang bị che bởi ? Brick
	// Vì dù Mario được push back bởi ? Brick, nhưng vẫn còn sự kiện Mario đã va chạm với Mushroom, nên Mushroom sẽ bị ăn bởi Mario
	// và bị delete mặc dù Mushroom vẫn chưa trồi lên
	if (e->obj->GetState() != MUSHROOM_STATE_IDLE)
	{
		//DebugOutTitle(L"Mario and Mushroom collided");
		e->obj->Delete();

		if (level == MARIO_LEVEL_SMALL)
		{
			SetLevel(MARIO_LEVEL_BIG);
		}
	}
}

void CMario::OnCllisionWithQuestionBrick(LPCOLLISIONEVENT e)
{
	if (e->ny > 0 &&
		e->obj->GetState() == QUESTION_BRICK_STATE_IDLE_HAVE_MUSHROOM)
	{
		e->obj->SetState(QUESTION_BRICK_STATE_BOUNCING_UP);
	}
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithWingGoomba(LPCOLLISIONEVENT e)
{
	CWingGoomba* wing_goomba = dynamic_cast<CWingGoomba*>(e->obj);

	if (e->ny < 0)	// Mario nhảy lên đầu Wing Goomba
	{
		if (wing_goomba->GetState() != WING_GOOMBA_STATE_DIE)
		{
			if (wing_goomba->GetLevel() == WING_GOOMBA_LEVEL_HAVE_WING)	// Nếu Wing Goomba đang level "có cánh" thì hạ level xuống thành "không cánh"
			{
				wing_goomba->SetLevel(WING_GOOMBA_LEVEL_NO_WING);	// Set level "không cánh"
				wing_goomba->SetState(WING_GOOMBA_STATE_WALKING);	// Vì đang là level "không cánh" nên chỉ có thể có chỉ số vật lý của state walking
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
			else  // Nếu Wing Goomba đang level "không cánh" thì cho die luôn
			{
				wing_goomba->SetState(WING_GOOMBA_STATE_DIE);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
		}
	}
	else  // Mario bị đụng bởi Wing Goomba
	{
		if (untouchable == 0)	// Nếu Mario có thể bị đụng vào
		{
			if (level > MARIO_LEVEL_SMALL)	// Mario đang ở level dạng Big thì chuyển lại thành level Small và bắt đầu khoảng thời gian Mario không thể bị đụng vào
			{
				SetLevel(MARIO_LEVEL_SMALL);
				StartUntouchable();
			}
			else  // Mario đang ở level Small thì cho chết luôn
			{
				DebugOut(L">>> Mario DIE >>> \n");
				SetState(MARIO_STATE_DIE);
			}
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;

	// Nếu đang chui vào pipe_gate hoặc chui ra khỏi pipe_des_out
	if (this->isGoingIntoPipeGate || this->isGettingOutOfPipeDesOut)
	{
		aniId = ID_ANI_MARIO_SMALL_PIPE;
		return aniId;
	}

	if (!isOnPlatform)
	{
		if (abs(vx) == MARIO_RUNNING_SPEED)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}

void CMario::GetAniIdRaccon()
{
	int ani_id = -1;

	float shift_x = 0;

	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);
	float width = right - left;
	float height = bottom - top;

	if (this->IsTailAttacking())
	{
		if (nx > 0)
			ani_id = ID_ANI_MARIO_RACCON_ATTACK_TAIL_RIGHT;
		else
			ani_id = ID_ANI_MARIO_RACCON_ATTACK_TAIL_LEFT;
	}

	// Nếu đang chui vào pipe_gate hoặc chui ra khỏi pipe_des_out
	if (this->isGoingIntoPipeGate || this->isGettingOutOfPipeDesOut)
	{
		ani_id = ID_ANI_MARIO_RACCON_PIPE;
		CAnimations::GetInstance()->Get(ani_id)->Render(x + (width + shift_x) / 2,
			y + height / 2);

		return;
	}

	if (!isOnPlatform)	// Đang trong không trung
	{
		// Nếu đang trong thời gian bay
		if (this->CanContinueFly())
		{
			// Nếu bay lên
			if (this->vy < 0)
			{
				if (nx > 0)
				{
					ani_id = ID_ANI_MARIO_RACCON_FLYING_RIGHT;
					shift_x = -5;
				}
				else
				{
					ani_id = ID_ANI_MARIO_RACCON_FLYING_LEFT;
					shift_x = 5;
				}
			}
			// Trong lúc bay thì có 2 ani, ani đầu tiên là khi bay lên, ani còn lại là dùng khi đang rơi nhưng vẫn đang trong thời gian bay
			// Nếu rơi xuống
			else if (abs(maxVx) == MARIO_SPEED_FLYING_X)
			{
				if (nx > 0)
				{
					ani_id = ID_ANI_MARIO_RACCON_JUMP_RUN_RIGHT;
					shift_x = -5;
				}
				else
				{
					ani_id = ID_ANI_MARIO_RACCON_JUMP_RUN_LEFT;
					shift_x = 5;
				}
			}
		}

		// Đang dùng đuôi để rơi chậm hơn
		else if (abs(maxVx) == MARIO_SPEED_FALL_SLOW_X &&
			vy == MARIO_SPEED_FALL_SLOW_Y)
		{
			if (nx > 0)
			{
				ani_id = ID_ANI_MARIO_RACCON_FALL_SLOW_RIGHT;
				shift_x = -7;
			}
			else
			{
				ani_id = ID_ANI_MARIO_RACCON_FALL_SLOW_LEFT;
				shift_x = 7;
			}
		}
		else if (abs(vx) == MARIO_RUNNING_SPEED) // Đang di chuyển với tốc độ nhanh (gia tốc chạy) trong không trung
		{
			if (nx > 0)
			{
				ani_id = ID_ANI_MARIO_RACCON_JUMP_RUN_RIGHT;
				shift_x = -5;
			}
			else
			{
				ani_id = ID_ANI_MARIO_RACCON_JUMP_RUN_LEFT;
				shift_x = 5;
			}
		}
		else  // Đang di chuyển với tốc độ bình thường (gia tốc đi bộ) trong không trung
		{
			if (nx > 0)
			{
				ani_id = ID_ANI_MARIO_RACCON_JUMP_WALK_RIGHT;
				shift_x = -7;
			}
			else
			{
				ani_id = ID_ANI_MARIO_RACCON_JUMP_WALK_LEFT;
				shift_x = 7;
			}
		}
	}
	else
	{
		if (isSitting)	// Đang ngồi
		{
			if (nx > 0)	// Đang quay mặt sang bên phải
			{
				ani_id = ID_ANI_MARIO_RACCON_SIT_RIGHT;
				shift_x = -7;
			}
			else        // Đang quay mặt sang bên trái
			{
				ani_id = ID_ANI_MARIO_RACCON_SIT_LEFT;
				shift_x = 7;
			}
		}
		else  // Đang không ngồi == Đang {đứng yên, đi bộ, chạy}
		{
			if (vx == 0)  // Đang đứng yên
			{
				if (nx >= 0)
				{
					ani_id = ID_ANI_MARIO_RACCON_IDLE_RIGHT;
					shift_x = -7; // dư 7 pixel
				}
				else
				{
					ani_id = ID_ANI_MARIO_RACCON_IDLE_LEFT;
					// Đối với sprite quay sang trái thì không cần dời pixel do dư cái đuôi,
					// nên chỉ cần lấy width và height của sprite chia đôi rồi vẽ như bình thường thôi, nhưng do cái bbox của Racccon thì lại
					// chỉ bằng Mario Big, nên cần phải cộng thêm 7 pixel dư từ cái đuôi nữa, thì chia đôi width và height thì mới ra đúng vị trí để vẽ từ top-left
					shift_x = 7;
				}

			}
			else if (vx > 0)	// Đang chạy sang bên phải
			{
				if (ax < 0)		// Đang thắng
					ani_id = ID_ANI_MARIO_RACCON_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)	// Đang chạy
				{
					ani_id = ID_ANI_MARIO_RACCON_RUNNING_RIGHT;
					shift_x = -6;
				}
				else if (ax == MARIO_ACCEL_WALK_X) // Đang đi bộ
				{
					ani_id = ID_ANI_MARIO_RACCON_WALKING_RIGHT;
					shift_x = -7;
				}
			}
			else  // Đang chạy sang bên trái
			{
				if (ax > 0)		// Đang thắng
					ani_id = ID_ANI_MARIO_RACCON_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)	// Đang chạy
				{
					ani_id = ID_ANI_MARIO_RACCON_RUNNING_LEFT;
					shift_x = 6;
				}
				else if (ax == -MARIO_ACCEL_WALK_X)	// Đang đi bộ
				{
					ani_id = ID_ANI_MARIO_RACCON_WALKING_LEFT;
					shift_x = 7;
				}
			}
		}
	}

	if (ani_id == -1)
		ani_id = ID_ANI_MARIO_RACCON_IDLE_RIGHT;

	CAnimations::GetInstance()->Get(ani_id)->Render(x + (width + shift_x) / 2,
		y + height / 2);
	RenderBoundingBox();
}

int CMario::GetAniIdBigHoldKoopa()
{
	int ani_id = -1;

	if (!isOnPlatform)
	{
		if (abs(vx) == MARIO_RUNNING_SPEED)
		{
			if (nx >= 0)
				ani_id = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				ani_id = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				ani_id = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				ani_id = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				ani_id = ID_ANI_MARIO_SIT_RIGHT;
			else
				ani_id = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) ani_id = ID_ANI_MARIO_HOLD_IDLE_RIGHT;
				else ani_id = ID_ANI_MARIO_HOLD_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					ani_id = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					ani_id = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					ani_id = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					ani_id = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					ani_id = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					ani_id = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (ani_id == -1)
		ani_id = ID_ANI_MARIO_HOLD_IDLE_RIGHT;

	return ani_id;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;

	// Nếu đang chui vào pipe_gate hoặc chui ra khỏi pipe_des_out
	if (this->isGoingIntoPipeGate || this->isGettingOutOfPipeDesOut)
	{
		aniId = ID_ANI_MARIO_PIPE;
		return aniId;
	}

	if (!isOnPlatform)
	{
		if (abs(vx) == MARIO_RUNNING_SPEED)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

void CMario::Render()
{
	int ani_id = -1;

	if (state == MARIO_STATE_DIE)	// Mario đã DIE
		ani_id = ID_ANI_MARIO_DIE;
	// Mario chưa DIE, vậy thì kiểm tra Level rồi lấy ra ani_id nằm trong những ani_id của level hiện tại,
	// rồi lấy ra ani_id phù hợp với các chỉ số vật lý hiện tại như: ax, nx và isSitting
	else
	{
		// Nếu đang cầm Koopa thì sẽ có các bộ ani riêng cho từng level
		if (isHoldingKoopa)
		{
			if (level == MARIO_LEVEL_BIG)
				ani_id = GetAniIdBigHoldKoopa();
			else if (level == MARIO_LEVEL_SMALL)
				ani_id = GetAniIdSmall();
			else if (level == MARIO_LEVEL_RACCON)
			{
				GetAniIdRaccon();
				return;
			}
		}
		else
		{
			if (level == MARIO_LEVEL_BIG)
				ani_id = GetAniIdBig();
			else if (level == MARIO_LEVEL_SMALL)
				ani_id = GetAniIdSmall();
			else if (level == MARIO_LEVEL_RACCON)
			{
				GetAniIdRaccon();
				return;
			}
		}
	}

	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);
	float width = right - left;
	float height = bottom - top + 1;

	CAnimations::GetInstance()->Get(ani_id)->Render(x + width / 2, y + height / 2);
	//CAnimations::GetInstance()->Get(ani_id)->Render(x, y);

	RenderBoundingBox();
	//DebugOutTitle(L"Mario: %0.2f, %0.2f, Bbox: %0.2f, %0.2f", x, y, left, top);

	//DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		// To-do:
		// Phải xét nếu Mario có đang đứng trên platform hay blocking object hay không, nếu có thì mới cho tăng tốc độ di chuyển (maxVx và ax) được (vật lý)
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		// To-do:
		// Phải xét nếu Mario có đang đứng trên platform hay blocking object hay không, nếu có thì mới cho tăng tốc độ di chuyển (maxVx và ax) được (vật lý)
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
			{
				vy = -MARIO_JUMP_SPEED_Y;
			}
		}
		else // Đang ở giữa không trung
		{
			// Nếu là level Raccon thì mới cho rơi chậm và bay
			if (this->GetLevel() == MARIO_LEVEL_RACCON)
			{
				/*if (abs(vx) == MARIO_RUNNING_SPEED)
					this->SetState(MARIO_STATE_FLYING);
				else*/
				this->SetState(MARIO_STATE_FALL_SLOW);
			}
		}
		break;

	case MARIO_STATE_FLYING:
		// Lực nhảy mạnh (khi chạy nhanh) của lần đầu nhấn nhảy thì đã có ở state jump rồi
		// Bây giờ là đang ở trên cao, nên chỉ cần cho Mario một lực bay nhỏ để bay lên mà thôi

		if (nx > 0)
			maxVx = MARIO_SPEED_FLYING_X;
		else
			maxVx = -MARIO_SPEED_FLYING_X;

		vy = -MARIO_SPEED_FLYING_Y;
		break;

	case MARIO_STATE_FALL_SLOW:
		if (nx > 0)
			maxVx = MARIO_SPEED_FALL_SLOW_X;
		else
			maxVx = -MARIO_SPEED_FALL_SLOW_X;

		vy = MARIO_SPEED_FALL_SLOW_Y;

		fallSlow_start = GetTickCount64();
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0;
			y += MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (level == MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x;
			top = y;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH - 1;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT - 1;
		}
		else
		{
			left = x;
			top = y;
			right = left + MARIO_BIG_BBOX_WIDTH - 1;
			bottom = top + MARIO_BIG_BBOX_HEIGHT - 1;
		}
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		left = x;
		top = y;
		right = left + MARIO_SMALL_BBOX_WIDTH - 1;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT - 1;
	}
	else if (level == MARIO_LEVEL_RACCON)
	{
		if (isSitting)
		{
			left = x;
			top = y;
			right = x + MARIO_BIG_SITTING_BBOX_WIDTH - 1;
			bottom = y + MARIO_BIG_SITTING_BBOX_HEIGHT - 1;
		}
		else
		{
			left = x;
			top = y;
			right = left + MARIO_BIG_BBOX_WIDTH - 1;
			bottom = top + MARIO_BIG_BBOX_HEIGHT - 1;
		}
	}

	//DebugOutTitle(L"Mario: %0.2f, %0.2f   BBox: %0.2f, %0.2f", x, y, top, left);
}

void CMario::GetWidth(float& width)
{
	float l, t, r, b;
	GetBoundingBox(l, t, r, b);
	width = r - l + 1;
}

void CMario::GetHeight(float& height)
{
	float l, t, r, b;
	GetBoundingBox(l, t, r, b);
	height = b - t + 1;
}

void CMario::SetLevel(int l)
{
	old_level = level;
	// Adjust position to avoid falling off platform
	// Nếu đang ở dạng Small, thì khi chuyển sang các dạng lớn hơn thì điều chỉnh lại vị trí bằng khoảng chênh lệch giữ dạng Small và dạng Big
	if (this->level == MARIO_LEVEL_SMALL &&
		l != MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT + 1 - MARIO_SMALL_BBOX_HEIGHT);
	}

	/*if (level == MARIO_LEVEL_BIG)
	{
		if (l == MARIO_LEVEL_RACCON)
		{
			if (nx > 0)
			{
				x -= 7;
			}
		}
	}


	if (level == MARIO_LEVEL_RACCON)
	{
		if (l == MARIO_LEVEL_BIG)
		{
			if (nx > 0)
			{
				x += 7;
			}
		}
	}*/

	level = l;
}

