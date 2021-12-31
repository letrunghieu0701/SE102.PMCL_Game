#include "Tail.h"


void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Nếu cái đuôi hết thời gian tồn tại thì xóa nó đi
	if (this->IsInLivingTime() == false)
	{
		this->Delete();
		return;
	}


	// Nếu vẫn còn sống thì cho trượt qua trượt lại để attack các object khác
	// 
	// Nếu đã trượt xong một khoảng bằng width của tail && tail chưa đổi hướng
	// Thì gán là đã đổi hướng và đổi hướng di chuyển cho tail (nx và vx)
	// Và phải đặt lại đúng vị trí max có thể di chuyển trong lần đầu trượt, để tiện cho việc xử lý về sau

	if (slide_direction < 0)
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
	}

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
	else if (e->obj->GetType() == OBJECT_TYPE_KOOPA)
		OnCollisionWithKoopa(e);
	else if (e->obj->GetType() == OBJECT_TYPE_QUESTION_BRICK)
		OnCollisionWithQuestionBrick(e);
	else if (e->obj->GetType() == OBJECT_TYPE_BREAKABLE_BRICK)
		OnColliswionWithBreakableBrick(e);
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

void CTail::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);

	if (koopa->GetState() == KOOPA_STATE_WALKING)
		koopa->StartBounce((int)e->nx);
	else if (koopa->GetState() == KOOPA_STATE_SHELLING)
		koopa->StartBounce((int)e->nx);

	koopa->SetShellDirection(KOOPA_SHELL_FACING_UP);
}

void CTail::OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e)
{
	if (e->obj->GetState() == QUESTION_BRICK_STATE_IDLE_HAVE_MUSHROOM)
	{
		e->obj->SetState(QUESTION_BRICK_STATE_BOUNCING_UP);
	}
}

void CTail::OnColliswionWithBreakableBrick(LPCOLLISIONEVENT e)
{
	e->obj->Delete();

	LPPLAYSCENE play_scene = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene());

	CCrumb* crumb_1 = new CCrumb(x, y, OBJECT_TYPE_CRUMB, GetTickCount64(), CRUMG_SHOOT_FORCE_STRONG_Y, CRUMB_SHOOT_RIGHT);
	CCrumb* crumb_2 = new CCrumb(x, y, OBJECT_TYPE_CRUMB, GetTickCount64(), CRUMG_SHOOT_FORCE_WEAK_Y, CRUMB_SHOOT_RIGHT);
	CCrumb* crumb_3 = new CCrumb(x, y, OBJECT_TYPE_CRUMB, GetTickCount64(), CRUMG_SHOOT_FORCE_STRONG_Y, CRUMB_SHOOT_LEFT);
	CCrumb* crumb_4 = new CCrumb(x, y, OBJECT_TYPE_CRUMB, GetTickCount64(), CRUMG_SHOOT_FORCE_WEAK_Y, CRUMB_SHOOT_LEFT);
	play_scene->AddGameObject(crumb_1);
	play_scene->AddGameObject(crumb_2);
	play_scene->AddGameObject(crumb_3);
	play_scene->AddGameObject(crumb_4);

	CBreakableBrick* bbrick = dynamic_cast<CBreakableBrick*>(e->obj);
	if (bbrick->IsWillBecomeNormalBrick())
	{
		float bbrick_x, bbrick_y;
		e->obj->GetPosition(bbrick_x, bbrick_y);
		CBrick* brick = new CBrick(bbrick_x, bbrick_y, OBJECT_TYPE_BRICK);
		play_scene->AddGameObject(brick);

		CPButton* p_button = new CPButton(bbrick_x, bbrick_y - PBUTTON_BBOX_HEIGHT, OBJECT_TYPE_PBUTTON);
		play_scene->AddGameObject(p_button);
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
