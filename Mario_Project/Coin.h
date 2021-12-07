#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

class CCoin : public CGameObject {
private:
	bool usedToBeBreakableBrick;
	ULONGLONG lifeTimeBecomeBreakableBrick;
public:
	CCoin(float x, float y, int type, bool wasBreakableBrick = false, ULONGLONG time = 0) : CGameObject(x, y, type)
	{
		this->usedToBeBreakableBrick = wasBreakableBrick;
		this->lifeTimeBecomeBreakableBrick = time;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};