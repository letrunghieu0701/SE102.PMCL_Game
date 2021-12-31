#pragma once
#include "GameObject.h"
#include "PipeGate.h"
#include "PipeTeleportDestination.h"
#include "Koopa.h"
#include "PButton.h"

#include "Platform.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

// Các chỉ số vật lý
#define MARIO_WALKING_SPEED			0.15f
#define MARIO_RUNNING_SPEED			0.4f

#define MARIO_ACCEL_WALK_X			0.0005f
#define MARIO_ACCEL_RUN_X			0.0007f
#define MARIO_FACTOR_AIR_FORCE_JUMPING_X	0.75F

#define MARIO_JUMP_SPEED_Y			0.6f
#define MARIO_JUMP_RUN_SPEED_Y		0.6f

#define MARIO_GRAVITY				0.002f
#define MARIO_FALL_DOWN_SPEED_Y		0.25f
#define MARIO_FALL_DOWN_2_PIPEGATE_SPEED_Y 0.02f

#define MARIO_JUMP_DEFLECT_SPEED	0.4f

// Raccon khi rơi chậm
// Lực cản không khí theo trục x khi di chuyển với tốc độ bình thường (đi bộ)
#define MARIO_AIR_FORCE_X				MARIO_WALKING_SPEED * 0.5f
// Lực cản không khí theo trục x khi di chuyển với tốc độ nhanh (chạy)
#define MARIO_AIR_FORCE_RUN_X			MARIO_RUNNING_SPEED * 0.5f

// Phản lực theo trục y, để rơi chậm hơn
#define MARIO_JET_FORCE_USING_TAIL_Y	MARIO_FALL_DOWN_SPEED_Y * 0.75f
// Lực cản không khí theo trục y, để không bay-lên-được-một-khoảng-cao
#define MARIO_AIR_FORCE_FLYING_Y		MARIO_JUMP_RUN_SPEED_Y * 0.2f

#define MARIO_SPEED_FALL_SLOW_X			(MARIO_WALKING_SPEED - MARIO_AIR_FORCE_X)
#define MARIO_SPEED_FALL_SLOW_Y			(MARIO_FALL_DOWN_SPEED_Y - MARIO_JET_FORCE_USING_TAIL_Y)

#define	MARIO_SPEED_FLYING_X			MARIO_SPEED_FALL_SLOW_X //(MARIO_RUNNING_SPEED - MARIO_AIR_FORCE_RUN_X)
#define MARIO_SPEED_FLYING_Y			(MARIO_JUMP_RUN_SPEED_Y - MARIO_AIR_FORCE_FLYING_Y)



// States
#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_FALL_SLOW		350

#define MARIO_STATE_FLYING			370

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601




#pragma region ANIMATION_ID

// Big Mario
#define ID_ANI_MARIO_IDLE_RIGHT			400
#define ID_ANI_MARIO_IDLE_LEFT			401

#define ID_ANI_MARIO_PIPE				410

#define ID_ANI_MARIO_WALKING_RIGHT		500
#define ID_ANI_MARIO_WALKING_LEFT		501

#define ID_ANI_MARIO_RUNNING_RIGHT		600
#define ID_ANI_MARIO_RUNNING_LEFT		601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT	700
#define ID_ANI_MARIO_JUMP_WALK_LEFT		701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT		800
#define ID_ANI_MARIO_JUMP_RUN_LEFT		801

#define ID_ANI_MARIO_SIT_RIGHT			900
#define ID_ANI_MARIO_SIT_LEFT			901

#define ID_ANI_MARIO_BRACE_RIGHT		1000
#define ID_ANI_MARIO_BRACE_LEFT			1001

#define ID_ANI_MARIO_DIE 999

// Big + Hold Koopa

#define ID_ANI_MARIO_HOLD_IDLE_RIGHT	1010
#define ID_ANI_MARIO_HOLD_IDLE_LEFT		1011


// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT		1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT		1101

#define ID_ANI_MARIO_SMALL_PIPE				1110

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT	1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT		1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT	1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT		1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT		1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT		1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT	1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT	1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT	1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT	1601


// Raccon Mario
#define	ID_ANI_MARIO_RACCON_IDLE_LEFT		1700
#define ID_ANI_MARIO_RACCON_IDLE_RIGHT		1701

#define ID_ANI_MARIO_RACCON_PIPE			1710

#define ID_ANI_MARIO_RACCON_WALKING_RIGHT	1800
#define ID_ANI_MARIO_RACCON_WALKING_LEFT	1801

#define	ID_ANI_MARIO_RACCON_RUNNING_RIGHT	1900
#define ID_ANI_MARIO_RACCON_RUNNING_LEFT	1901

#define ID_ANI_MARIO_RACCON_JUMP_WALK_RIGHT	2000
#define ID_ANI_MARIO_RACCON_JUMP_WALK_LEFT	2001

#define ID_ANI_MARIO_RACCON_JUMP_RUN_RIGHT	2100
#define ID_ANI_MARIO_RACCON_JUMP_RUN_LEFT	2101

#define ID_ANI_MARIO_RACCON_SIT_RIGHT		2200
#define ID_ANI_MARIO_RACCON_SIT_LEFT		2201

#define ID_ANI_MARIO_RACCON_BRACE_RIGHT		2300
#define ID_ANI_MARIO_RACCON_BRACE_LEFT		2301

#define ID_ANI_MARIO_RACCON_FALL_SLOW_RIGHT	2400
#define ID_ANI_MARIO_RACCON_FALL_SLOW_LEFT	2401

#define	ID_ANI_MARIO_RACCON_FLYING_RIGHT	2500
#define ID_ANI_MARIO_RACCON_FLYING_LEFT		2501

#define ID_ANI_MARIO_RACCON_ATTACK_TAIL_RIGHT	2600
#define ID_ANI_MARIO_RACCON_ATTACK_TAIL_LEFT	2601

#pragma endregion


// Cần xem xét sau
#define GROUND_Y 160.0f


#define MARIO_PUSH_DOWN 26
#define MARIO_PUSH_HORIZONTAL 12



// Level
#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_RACCON	3




// Bouding box
#define MARIO_SMALL_BBOX_WIDTH  12
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_RACCON_BBOX_WIDTH  14
#define MARIO_RACCON_BBOX_HEIGHT 28

#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 18




// Điều chỉnh độ cao khi ngồi
#define MARIO_SIT_HEIGHT_ADJUST (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_SITTING_BBOX_HEIGHT)




// Các giới hạn thời gian
#define MARIO_UNTOUCHABLE_TIME	2500

#define MARIO_FALL_SLOW_TIME	200

#define MARIO_FLYING_TIME		2000

#define MARIO_ATTACK_TAIL_TIME	200

#define MARIO_TURN_OFF_COLLISION_TIME 50


class CMario : public CGameObject
{
private:
	BOOLEAN isSitting;
	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	int level; 
	int untouchable; 
	ULONGLONG untouchable_start;
	BOOLEAN isOnPlatform;

	int coin;

	bool isStopUpdate;

	ULONGLONG fallSlow_start;
	ULONGLONG flying_start;

	bool isTailAttacking;
	ULONGLONG attackTail_start;

	bool isOnPipeGate;
	bool canGoIntoPipeGate;
	bool is_pressing_up_button;
	bool turn_off_collision;

	ULONGLONG turnOffCollision_start;

	bool isGoingIntoPipeGate;
	float oldPos_y;
	CPipeGate* current_pipegate;

	bool isGettingOutOfPipeDesOut; // Có phải là đang chui ra khỏi pipe_des ở mặt đất hay không
	CPipeTeleportDestination* current_pipedes;

	int old_level;

	bool isPressingHoldKoopaButton;
	bool isHoldingKoopa;

	CKoopa* current_koopa_holding;


	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWithWingGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithPipeGate(LPCOLLISIONEVENT e);
	void OnCollisionWithPButton(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();
	void GetAniIdRaccon();

	int GetAniIdBigHoldKoopa();

public:
	CMario(float x, float y, int type) : CGameObject(x, y, type)
	{
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY; 

		level = old_level = MARIO_LEVEL_RACCON;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;

		coin = 0;

		isStopUpdate = false;

		fallSlow_start = 0;
		flying_start = 0;

		isTailAttacking = false;
		attackTail_start = 0;

		isOnPipeGate = false;
		canGoIntoPipeGate = false;

		is_pressing_up_button = false;

		turn_off_collision = false;
		turnOffCollision_start = 0;

		isGoingIntoPipeGate = false;
		oldPos_y = 0;
		current_pipegate = nullptr;

		isGettingOutOfPipeDesOut = false;
		current_pipedes = nullptr;

		isHoldingKoopa = false;
		isPressingHoldKoopaButton = false;
		current_koopa_holding = nullptr;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable()
	{ 
		return (state != MARIO_STATE_DIE); 
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable==0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(int l);
	int GetLevel() { return this->level; }
	
	void StartCanFly(ULONGLONG time) { this->flying_start = time; }
	ULONGLONG GetFlyingStart() { return this->flying_start; }

	bool CanContinueFly() {
		// Nếu đã tiếp đất rồi thì không cho bay nữa
		if (this->isOnPlatform)
		{
			flying_start = 0;

			return false;
		}
			
		ULONGLONG time_passed = GetTickCount64() - this->GetFlyingStart();
		return (0 <= time_passed && time_passed <= MARIO_FLYING_TIME);
	}

	void AttackWithTail()
	{ 
		this->attackTail_start = GetTickCount64();
		isTailAttacking = true;
	}

	bool IsTailAttacking()
	{
		return ((0 <= (GetTickCount64() - this->attackTail_start)) &&
			((GetTickCount64() - this->attackTail_start) <= MARIO_ATTACK_TAIL_TIME));
	}

	BOOLEAN IsOnPlatform() { return this->isOnPlatform; }

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	bool IsStopUpdate() { return isStopUpdate; }

	bool IsOnPipeGate() { return this->isOnPipeGate; }
	void ReadyToGoIntoPipeGate(bool is_ready) { this->canGoIntoPipeGate = is_ready; }
	bool CanGoIntoPipeGate() { return this->canGoIntoPipeGate; }

	void IsPressingUpButton(bool is_pressing) { this->is_pressing_up_button = is_pressing; }

	void StartTurnOffCollision() {
		this->ReadyToGoIntoPipeGate(false);
		this->turn_off_collision = true;
		this->turnOffCollision_start = GetTickCount64();
		DebugOut(L"Tắt collision\n");
	}
	bool IsTurningOffCollision()
	{
		ULONGLONG time_passed = GetTickCount64() - this->turnOffCollision_start;
		return (0 <= time_passed &&
			time_passed <= MARIO_TURN_OFF_COLLISION_TIME);
	}

	CPipeGate* GetCurrentPipeGate() { return this->current_pipegate; }

	bool IsGoingIntoPipeGate() { return this->isGoingIntoPipeGate; }
	bool IsGettingOutOfPipeDes() { return this->isGettingOutOfPipeDesOut; }

	void SetHoldKoopaButton(bool is_pressing_button) { this->isPressingHoldKoopaButton = is_pressing_button; }

	void GetWidth(float &width);;
	void GetHeight(float &height);

	void GetMaxSpeedX(float& maxv_x) { maxv_x = this->maxVx; }
};