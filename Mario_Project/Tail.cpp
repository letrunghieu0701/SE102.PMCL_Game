#include "Tail.h"


void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Nếu cái đuôi hết thời gian tồn tại thì xóa nó đi
	/*if (this->StillHaveLivingTimeLeft() == false)
	{
		this->Delete();
		return;
	}*/

	// Nếu đã trượt xong một khoảng bằng width của tail && tail chưa đổi hướng
	// Thì gán là đã đổi hướng và đổi hướng di chuyển cho tail (nx và vx)
	// Và phải đặt lại đúng vị trí max có thể di chuyển trong lần đầu trượt, để tiện cho việc xử lý về sau

	/*if (slide_direction < 0)
	{
		if (x < max_travel_distance_left && turn_back == false)
		{
			this->turn_back = true;
			this->x = max_travel_distance_left;
			nx = -nx;
			vx = -vx;
		}

		if (x > max_travel_distance_right && turn_back == true)
		{
			this->Delete();
		}
	}
	else
	{
		if (x > max_travel_distance_right && turn_back == false)
		{
			this->turn_back = true;
			this->x = max_travel_distance_right;
			nx = -nx;
			vx = -vx;
		}

		if (x < max_travel_distance_left && turn_back == true)
		{
			this->Delete();
		}
	}*/


	// Nếu vẫn còn sống thì cho trượt qua trượt lại để attack các object khác
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CTail::OnNoCollision(DWORD dt)
{
	x += vx * dt;
}

void CTail::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->GetType() == OBJECT_TYPE_GOOMBA)
		OnCollisionWithGoomba(e);
	else if (e->obj->GetType() == OBJECT_TYPE_WING_GOOMBA)
		OnCollisionWithWingGoomba(e);
}

void CTail::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	if (goomba->GetState() != GOOMBA_STATE_DIE)
	{
		goomba->SetState(GOOMBA_STATE_DIE);
	}
}

void CTail::OnCollisionWithWingGoomba(LPCOLLISIONEVENT e)
{
	CWingGoomba* wing_goomba = dynamic_cast<CWingGoomba*>(e->obj);

	if (wing_goomba->GetState() != WING_GOOMBA_STATE_DIE)
	{
		if (wing_goomba->GetLevel() == WING_GOOMBA_LEVEL_HAVE_WING)	// Nếu Wing Goomba đang level "có cánh" thì hạ level xuống thành "không cánh"
		{
			wing_goomba->SetLevel(WING_GOOMBA_LEVEL_NO_WING);	// Set level "không cánh"
			wing_goomba->SetState(WING_GOOMBA_STATE_WALKING);	// Vì đang là level "không cánh" nên chỉ có thể có chỉ số vật lý của state walking
		}
		else  // Nếu Wing Goomba đang level "không cánh" thì cho die luôn
		{
			wing_goomba->SetState(WING_GOOMBA_STATE_DIE);
		}
	}
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
