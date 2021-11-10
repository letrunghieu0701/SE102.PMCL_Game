#include "ChangeDirectionOnPlatform.h"

#include "Koopa.h"


CChangeDirectionOnPlatform::CChangeDirectionOnPlatform(float x, float y, int type, int id) : CGameObject(x, y, type)
{
	this->ay = CDOP_SPEED_GRAVITY;
	this->isOnPlatform = false;
	this->id = id;
}

void CChangeDirectionOnPlatform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;

	this->isOnPlatform = false;

	CCollision::GetInstance()->Process(this, dt, coObjects);
	//DebugOutTitle(L"CDOP: x: %d y: %d", x, y);
	//DebugOutTitle(L"CDOP: vx: %0.2f vy: %0.2f", vx, vy);
	//DebugOutTitle(L"CDOP: On Platform: %d", isOnPlatform);
}


void CChangeDirectionOnPlatform::OnNoCollision(DWORD dt)
{
	y += vy * dt;
}

void CChangeDirectionOnPlatform::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->IsBlocking())
	{
		this->vy = 0;

		if (e->ny == DIRECTION_UP)
		{
			this->isOnPlatform = true;
		}
	}

	if (e->obj->GetType() == OBJECT_TYPE_KOOPA)
	{
		OnCollisionWithKoopa(e);
		DebugOut(L"Phát hiện va chạm giữa CDOP và Koopa \n");
	}
	else
	{
		DebugOut(L">>> Không phát hiện va chạm >>> \n");
	}
		
}

void CChangeDirectionOnPlatform::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	
	//if (e->nx != NO_COLLISION_ON_THIS_AXIS)
	//{
	//	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	//	float koopa_vx, koopa_vy;
	//	float koopa_x, koopa_y;
	//	koopa->GetSpeed(koopa_vx, koopa_vy);
	//	koopa->GetPosition(koopa_x, koopa_y);

	//	// Tính phần khoảng cách overlap giữa CDOP và Koopa để lát nữa đẩy CDOP bằng khoảng cách này, đẩy theo hướng va chạm
	//	// Cộng thêm một chút push back để không bị overlap ở frame kế tiếp
	//	/*float overlap_distance_on_x = (e->dt - e->t) * koopa_vx + 
	//									e->nx * (3+BLOCK_PUSH_FACTOR);*/

	//	float overlap_distance_on_x = this->x - koopa_x + e->nx * BLOCK_PUSH_FACTOR;

	//	this->SetPosition(koopa_x + KOOPA_BBOX_SHELL_WIDTH + e->nx * 2, y);
	//	DebugOutTitle(L"Vector normal: x: %0.f", e->nx);
	//}
}

void CChangeDirectionOnPlatform::Render()
{
	int ani_id = ID_ANI_CDOP;

	CAnimations::GetInstance()->Get(ani_id)->Render(x, y);
	RenderBoundingBox();
}

void CChangeDirectionOnPlatform::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - CDOP_BBOX_WIDTH / 2;
	top = y - CDOP_BBOX_HEIGHT / 2;
	right = left + CDOP_BBOX_WIDTH;
	bottom = top + CDOP_BBOX_HEIGHT;
}