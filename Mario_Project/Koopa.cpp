#include "Koopa.h"

#include "Goomba.h"
#include "WingGoomba.h"
#include "ChangeDirectionOnPlatform.h"

#include "PlayScene.h"

CKoopa::CKoopa(float x, float y, int type, bool can_turn, int id_CDOP) : CGameObject(x, y, type)
{
	ax = 0;
	ay = KOOPA_SPEED_GRAVITY;
	
	this->id_CDOP = id_CDOP;

	current_state = KOOPA_STATE_WALKING;
	SetState(current_state);
	if (vx > 0)
		nx = DIRECTION_RIGHT;
	else
		nx = DIRECTION_LEFT;

	bounce_start = 0;
	hit_direction = 0;
	hit_by_mario = false;

	// Nếu đây là Koopa có thể quay đầu trên platform trong không trung
	if (this->id_CDOP != -1)
	{
		// Đặt CDOP về trước mặt Koopa
		unordered_map<int, LPGAMEOBJECT> item_list = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetItemList();
		CChangeDirectionOnPlatform* CDOP = dynamic_cast<CChangeDirectionOnPlatform*>(item_list[this->id_CDOP]);
		if (CDOP == NULL)
			return;

		float new_CDOP_x = this->x + (this->GetNormalDirectionX() * (KOOPA_BBOX_WALKING_WIDTH + BLOCK_PUSH_FACTOR));
		CDOP->SetPosition(new_CDOP_x, this->y);
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx += ax * dt;
	vy += ay * dt;

	if (vy >= KOOPA_FALL_DOWN_SPEED_Y)
		vy = KOOPA_FALL_DOWN_SPEED_Y;

	int current_state = this->GetState();

	// Nếu đã hết thời gian nảy và đã từng bị đánh bởi Mario
	// Thì cho Koopa về state shelling tiếp, rồi để các thông số liên quan đến bounce về mặc định
	if (!IsInBounceTime() && hit_by_mario)
	{
		SetState(KOOPA_STATE_SHELLING);
		hit_by_mario = false;
		bounce_start = 0;
		hit_direction = 0;
	}


	// Nếu Koopa đang trong state đi bộ thì mới có thể làm quay đầu khi đi tới rìa platform, vì nếu đang trong state spin shell mà lại
	// có thể quay đầu được thì sẽ khiến game khá khó chịu nếu Koopa cứ xoay xoay hoài ở trên platform mà nó đang xoay

	if (current_state == KOOPA_STATE_WALKING)
	{
		if (this->id_CDOP != -1)
		{
			unordered_map<int, LPGAMEOBJECT> item_list = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetItemList();
			CChangeDirectionOnPlatform* CDOP = dynamic_cast<CChangeDirectionOnPlatform*>(item_list[this->id_CDOP]);
			if (CDOP == NULL)
				return;

			float CDOP_x, CDOP_y;
			CDOP->GetPosition(CDOP_x, CDOP_y);

			// Nếu chênh lệch độ cao quá lớn giữa Koopa và CDOP thì:
			// Quay Koopa lại (đổi hướng di chuyển): đổi chiều vận tốc vx và vector normal nx
			// Đặt CDOP về phía trước Koopa
			if (abs(this->y - CDOP_y) > MAX_DISTANCE_ON_Y_BETWEEN_KOOPA_CDOP)
			{
				this->vx = -vx;
				this->SetNormalDirectionX(-GetNormalDirectionX());

				float new_CDOP_x = this->x + this->GetNormalDirectionX() * (KOOPA_BBOX_WALKING_WIDTH + BLOCK_PUSH_FACTOR);
				CDOP->SetPosition(new_CDOP_x, this->y);
			}
			// Nếu cả hai không cách quá xa nhau về độ cao, thì Koopa cứ đẩy CDOP theo hướng di chuyển hiện tại của Koopa thôi
			// Thực ra ở đây chỉ là xét cứng vị trí của CDOP theo Koopa mà thôi, vẫn chưa đẩy được CDOP
			else
			{
				float new_CDOP_x = this->x + this->GetNormalDirectionX() * (KOOPA_BBOX_WALKING_WIDTH + BLOCK_PUSH_FACTOR);
				CDOP->SetPosition(new_CDOP_x, CDOP_y);
			}
		}
	}
	// Nếu Koopa đã trốn trong mai rùa quá lâu thì cho Koopa chui ra
	else if (current_state == KOOPA_STATE_SHELLING && this->IsCanInShell() == false)
		this->SetState(KOOPA_STATE_WALKING);



	CCollision::GetInstance()->Process(this, dt, coObjects);
	//DebugOutTitle(L"Koopa: vx: %0.2f vy: %0.2f ax: %0.2f ay: %0.2f", vx, vy, ax, ay);
	//DebugOutTitle(L"Koopa speed: vx: %0.2f vy: %0.2f", vx, vy);
	//DebugOutTitle(L"Koopa state: %d", state);
}

int CKoopa::GetAniIdWalk()
{
	int ani_id = -1;

	if (nx > 0)
		ani_id = ID_ANI_KOOPA_WALKING_RIGHT;
	else
		ani_id = ID_ANI_KOOPA_WALKING_LEFT;

	if (ani_id == -1)
		ani_id = ID_ANI_KOOPA_WALKING_RIGHT;

	return ani_id;
}

int CKoopa::GetAniIdSpinShell()
{
	int ani_id = -1;

	if (nx > 0)
		ani_id = ID_ANI_KOOPA_SPIN_SHELL_RIGHT;
	else
		ani_id = ID_ANI_KOOPA_SPIN_SHELL_LEFT;

	if (ani_id == -1)
		ani_id = ID_ANI_KOOPA_SPIN_SHELL_RIGHT;

	return ani_id;
}

void CKoopa::Render()
{
	int ani_id = ID_ANI_KOOPA_SPIN_SHELL_RIGHT;

	if (GetState() == KOOPA_STATE_WALKING)
		ani_id = GetAniIdWalk();
	else if (GetState() == KOOPA_STATE_SHELLING)
		ani_id = ID_ANI_KOOPA_SHELLING;
	else if (GetState() == KOOPA_STATE_HOLDED_BY_MARIO)
		ani_id = ID_ANI_KOOPA_SHELLING;
	else if (GetState() == KOOPA_STATE_SPIN_SHELL)
		ani_id = GetAniIdSpinShell();
	else if (GetState() == KOOPA_STATE_SHELL_BOUNCE_UP)
		ani_id = ID_ANI_KOOPA_SHELLING;


	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);
	float width = right - left;
	float height = bottom - top;

	CAnimations::GetInstance()->Get(ani_id)->Render(x + width / 2, y + height / 2);
	RenderBoundingBox();
}

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->IsBlocking())
	{
		if (e->ny != 0) // Va chạm với Blocking object theo trục y, thì đặt lại vy = 0 để mô phỏng vật lý cơ bản
		{
			this->vy = 0;
		}
		else if (e->nx != 0) // Nếu va chạm với Blocking object theo trục x, thì quay đầu lại và đi tiếp (cho vận tốc và normal vector ngược dấu)
		{
			this->vx = -vx;
			this->nx = -nx;
		}
	}

	if (e->obj->GetType() == OBJECT_TYPE_GOOMBA)
		OnCollisionWithGoomba(e);
	else if (e->obj->GetType() == OBJECT_TYPE_WING_GOOMBA)
		OnCollisionWithWingGoomba(e);
}

void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	// Koopa đang ở state Spin Shell thì mới có thể tấn công Goomba được
	if (this->GetState() == KOOPA_STATE_SPIN_SHELL)
	{
		CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

		// Nếu Goomba đang ở trạng thái khác DIE, tức là Goomba còn sống, thì cho nó die luôn
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
		}
		// Còn nếu Goomba đang ở state DIE, thì thôi, không làm gì cả, vì nó đang ở state DIE rồi mà
	}
}

void CKoopa::OnCollisionWithWingGoomba(LPCOLLISIONEVENT e)
{
	// Koopa đang ở state Spin Shell thì mới có thể tấn công Wing Goomba được
	if (this->GetState() == KOOPA_STATE_SPIN_SHELL)
	{
		CWingGoomba* wing_goomba = dynamic_cast<CWingGoomba*>(e->obj);

		// Nếu Wing Goomba còn 2 mạng (level có-cánh), thì giảm một mạng (level thành không-cánh) và gắn state thành WALKING (vì đâu có còn cánh đâu mà bay)
		if (wing_goomba->GetLevel() == WING_GOOMBA_LEVEL_HAVE_WING)
		{
			wing_goomba->SetLevel(WING_GOOMBA_LEVEL_NO_WING);
			wing_goomba->SetState(WING_GOOMBA_STATE_WALKING);
		}
		// Nếu Wing Goomba chỉ còn 1 mạng (level không-cánh), thì cho die luôn
		else if (wing_goomba->GetLevel() == WING_GOOMBA_LEVEL_NO_WING)
		{
			wing_goomba->SetState(WING_GOOMBA_STATE_DIE);
		}
		// Nếu Wing Goomba đã die rồi, thì thôi, không làm gì cả, vì nó DIE rồi mà
	}
}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case KOOPA_STATE_WALKING:
	{
		// Nếu đang ở trong mai rùa và giờ chui ra để đi bộ
		// Thì phải set lại vị trí để không rơi khỏi platform
		if (current_state == KOOPA_STATE_SHELLING)
		{
			this->y -= KOOPA_SHELL_2_WALK_HEIGHT_ADJUST;
		}
		ay = KOOPA_SPEED_GRAVITY;

		// Cho nhảy lên để hạn chế việc bị overlap với platform bên dưới
		vy = -KOOPA_SPEED_JUMP_DEFLECT;

		if (nx > 0)
			vx = KOOPA_SPEED_WALKING;
		else
			vx = -KOOPA_SPEED_WALKING;
		break;
	}
	case KOOPA_STATE_SHELLING:
	{
		vx = 0;
		ay = KOOPA_SPEED_GRAVITY;

		shell_start = GetTickCount64();
		break;
	}
	case KOOPA_STATE_HOLDED_BY_MARIO:
	{
		ay = 0;
		vx = 0;
		vy = 0;
		break;
	}
	case KOOPA_STATE_SPIN_SHELL:
	{
		if (GetNormalDirectionX() == DIRECTION_RIGHT)
			vx = KOOPA_SPEED_SPINNING;
		else
			vx = -KOOPA_SPEED_SPINNING;

		ay = KOOPA_SPEED_GRAVITY;

		// Nếu Mario va chạm trên đầu Koopa đang trong state spin shell thì có thể khiến Koopa trở về state shell,
		// Sau đó Koopa có thể chui ra khỏi shell và đi bộ (trở lại state walking)
		// To-do: cần suy nghĩ xem chuyện gì sẽ xảy ra với CDOP khi thêm feature này vào game

		/*unordered_map<int, LPGAMEOBJECT> item_list = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetItemList();
		CChangeDirectionOnPlatform* CDOP = dynamic_cast<CChangeDirectionOnPlatform*>(item_list[this->id_CDOP]);
		if (CDOP == NULL)
			return;
		CDOP->Delete();*/
		break;
	}
	case KOOPA_STATE_SHELL_BOUNCE_UP:
	{
		// Bị đánh từ bên trái, vì cho nảy qua bên phải và ngược lại
		if (hit_direction < 0)
			vx = KOOPA_SPEED_BOUNCE_X;
		else
			vx = -KOOPA_SPEED_BOUNCE_X;

		vy = -KOOPA_SPEED_BOUNCE_Y;
		ay = KOOPA_SPEED_GRAVITY;
		break;
	}
	}
	current_state = state;
	DebugOut(L"Koopa đang trong state: %d \n", this->state);
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (this->GetState() == KOOPA_STATE_WALKING)
	{
		left = x;
		top = y;
		right = left + KOOPA_BBOX_WALKING_WIDTH;
		bottom = top + KOOPA_BBOX_WALKING_HEIGHT;
	}
	else
	{
		left = x;
		top = y;
		right = left + KOOPA_BBOX_SHELL_WIDTH;
		bottom = top + KOOPA_BBOX_SHELL_HEIGHT;
	}
}