#include "Collision.h"
#include "GameObject.h"

CCollision* CCollision::__instance = NULL;

CCollision* CCollision::GetInstance()
{
	if (__instance == NULL) __instance = new CCollision();
	return __instance;
}

/*
	SweptAABB
*/
void CCollision::SweptAABB(
	float ml, float mt, float mr, float mb,
	float dx, float dy,
	float sl, float st, float sr, float sb,
	float& t, float& nx, float& ny)
{

	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	t = -1.0f;			// no collision
	nx = ny = 0;

	//
	// Broad-phase test 
	//

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;


	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	if (dx > 0)
	{
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}


	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0)
	{
		tx_entry = -9999999.0f;
		tx_exit = 99999999.0f;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -99999999999.0f;
		ty_exit = 99999999999.0f;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}


	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}

}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CCollision::SweptAABB(LPGAMEOBJECT objSrc, DWORD dt, LPGAMEOBJECT objDest)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	float mvx, mvy;
	objSrc->GetSpeed(mvx, mvy);
	float mdx = mvx * dt;
	float mdy = mvy * dt;

	float svx, svy;
	objDest->GetSpeed(svx, svy);
	float sdx = svx * dt;
	float sdy = svy * dt;

	//
	// NOTE: new m speed = original m speed - collide object speed
	// 
	float dx = mdx - sdx;
	float dy = mdy - sdy;

	objSrc->GetBoundingBox(ml, mt, mr, mb);
	objDest->GetBoundingBox(sl, st, sr, sb);

	SweptAABB(
		ml, mt, mr, mb,
		dx, dy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, dt, dx, dy, objDest, objSrc);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
/*
	"potetial" là bởi vì source object sẽ va chạm với cả đống object, nhưng ko phải cái nào cũng là cái mà source object va chạm đầu tiên,
	nên cần phải lưu hết lại để về sau xử lý, cụ thể là chạy hàm Filter để lọc ra những object va chạm đầu tiên với source object
*/
void CCollision::Scan(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* objDests, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < objDests->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABB(objSrc, dt, objDests->at(i));

		if (e->WasCollided() == 1)
			coEvents.push_back(e);
		else
			delete e;
	}

	//std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

/* Filter là để lựa ra những object nào mà source object va chạm đầu tiên.
	Bởi vì rất có thể là source object sẽ đi va chạm với rất nhiều object ở frame trước đó, và những object đó sẽ được thêm vào danh sách
	colliision event để xử lý, mà do xử lý va chạm là chỉ xử lý với những object mình va chạm đầu tiên thôi, tụi kia là do nó nằm trên quỹ đạo
	di chuyển của source object nên mới bị thêm vào danh sách collision event. Do đó, mình cần phải filter (lọc) ra những collision event đầu tiên.

	Hàm này có chức năng chính là Filter (lọc và giữ lại) va chạm đầu tiên trên cả 2 trục x, y
	(hoặc có thể chọn Filter trên một trục x hoặc y, nếu cần thiết)
	Ta còn có thể tùy chỉnh tham số filterBLock để có thể filter những Blocking object hay không
 */
void CCollision::Filter(LPGAMEOBJECT objSrc,
	vector<LPCOLLISIONEVENT>& coEvents,
	LPCOLLISIONEVENT& colX,
	LPCOLLISIONEVENT& colY,
	int filterBlock = 1,		// 1 = only filter block collisions, 0 = filter all collisions 
	int filterX = 1,			// 1 = process events on X-axis, 0 = skip events on X 
	int filterY = 1)			// 1 = process events on Y-axis, 0 = skip events on Y
{
	float min_tx, min_ty;

	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	for (UINT i = 0; i < coEvents.size(); i++)
	{

		LPCOLLISIONEVENT c = coEvents[i];
		if (c->isDeleted) continue;
		if (c->obj->IsDeleted()) continue;

		/*if (c->obj->GetType() == OBJECT_TYPE_INVISIBLE_PLATFORM)
		{
			DebugOutTitle(L"object type: invisible platform");;
		}*/

		// ignore collision event with object having IsBlocking = 0 (like coin, mushroom, etc)
		// Giữ lại đoạn code này để có thể debug, mục đích là có thể nó sẽ giúp ích giải thích các hiện tượng lạ (bug) khác
		//if (filterBlock == 1 &&
		//	(!c->obj->IsBlocking() /* ||
		//		!(c->nx==0 && c->ny<0 &&	// This condition make platform object can't be touch from x-axis
		//		c->obj->GetType()==OBJECT_TYPE_INVISIBLE_PLATFORM)*/))	// This condition make platform object can't be touch, thus Mario fall off the platform
		//{
		//	continue;
		//}

		// Filter (giữ lại) event này nếu:
		// 1. Đây là blocking object
		// 2.1. Đây là Invisible platform object
		// 2.2. Source object va chạm với top của Invisible platform
		if (filterBlock == 1)
		{
			// Khi va chạm với InviPlat thì chỉ giữ lại event va chạm với top của InviPlat để xử lý push back mà thôi
			// Các hướng còn lại thì bỏ hết, bỏ sạch luôn, nên chỉ cần lệnh if này là có thể thực hiện platform có thể nhảy từ dưới lên (One-way platform)
			if ((c->obj->GetType() == OBJECT_TYPE_INVISIBLE_PLATFORM) && !(c->ny == -1))
				continue;
			// Giữ lại blocking object
			if (!c->obj->IsBlocking())
				continue;
		}

		if (filterX == 1 &&
			c->nx != 0 &&
			c->t < min_tx)
		{
			min_tx = c->t; min_ix = i;
		}

		if (filterY == 1 &&
			c->ny != 0 &&
			c->t < min_ty)
		{
			min_ty = c->t; min_iy = i;
		}
	}

	if (min_ix >= 0) colX = coEvents[min_ix];
	if (min_iy >= 0) colY = coEvents[min_iy];
}

/*
*  Simple/Sample collision framework
*  NOTE: Student might need to improve this based on game logic
*/
void CCollision::Process(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	LPCOLLISIONEVENT colX = NULL;
	LPCOLLISIONEVENT colY = NULL;

	coEvents.clear();

	if (objSrc->IsCollidable())
	{
		Scan(objSrc, dt, coObjects, coEvents);
	}

	// No collision detected
	if (coEvents.size() == 0)
	{
		objSrc->OnNoCollision(dt);
	}
	else
	{
		Filter(objSrc, coEvents, colX, colY);

		float x, y, vx, vy, dx, dy;
		objSrc->GetPosition(x, y);
		objSrc->GetSpeed(vx, vy);
		dx = vx * dt;
		dy = vy * dt;



		if (colX != NULL && colY != NULL)
		{
			if (colY->t < colX->t)	// was collision on Y first ?
			{
				y += colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;
				objSrc->SetPosition(x, y);

				objSrc->OnCollisionWith(colY);

				//
				// see if after correction on Y, is there still a collision on X ? 
				//
				LPCOLLISIONEVENT colX_other = NULL;

				//
				// check again if there is true collision on X 
				//
				colX->isDeleted = true;		// remove current collision event on X

				// replace with a new collision event using corrected location 
				coEvents.push_back(SweptAABB(objSrc, dt, colX->obj));

				// re-filter on X only
				Filter(objSrc, coEvents, colX_other, colY, /*filterBlock=*/ 1, /*filterX=*/ 1, /*filterY=*/ 0);

				if (colX_other != NULL)
				{
					x += colX_other->t * dx + colX_other->nx * BLOCK_PUSH_FACTOR;
					objSrc->OnCollisionWith(colX_other);
				}
				else
				{
					x += dx;
				}
			}
			else // collision on X first
			{
				x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;
				objSrc->SetPosition(x, y);

				objSrc->OnCollisionWith(colX);

				//
				// see if after correction on X, is there still a collision on Y ? 
				//
				LPCOLLISIONEVENT colY_other = NULL;

				//
				// check again if there is true collision on Y
				//
				colY->isDeleted = true;		// remove current collision event on Y

				// replace with a new collision event using corrected location 
				coEvents.push_back(SweptAABB(objSrc, dt, colY->obj));

				// re-filter on Y only
				Filter(objSrc, coEvents, colX, colY_other, /*filterBlock = */ 1, /*filterX=*/0, /*filterY=*/1);

				if (colY_other != NULL)
				{
					y += colY_other->t * dy + colY_other->ny * BLOCK_PUSH_FACTOR;
					objSrc->OnCollisionWith(colY_other);
				}
				else
				{
					y += dy;
				}
			}
		}
		else if (colX != NULL)
		{
			x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;
			y += dy;
			objSrc->OnCollisionWith(colX);
		}
		else if (colY != NULL)
		{
			x += dx;
			y += colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;
			objSrc->OnCollisionWith(colY);
			
		}
		else // both colX & colY are NULL 
		{
			x += dx;
			y += dy;
		}

		objSrc->SetPosition(x, y);
	}

	//
	// Scan all non-blocking collisions for further collision logic
	//
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT e = coEvents[i];
		if (e->isDeleted) continue;
		if (e->obj->IsBlocking()) continue;  // blocking collisions were handled already, skip them

		objSrc->OnCollisionWith(e);
	}


	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
