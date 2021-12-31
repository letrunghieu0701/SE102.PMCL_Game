#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_HUD 16000
#define HUD_BBOX_WIDTH 153
#define HUD_BBOX_HEIGHT 30

class CHUD : public CGameObject {
public:
	CHUD(float x, float y, int type) : CGameObject(x, y, type) {}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetState(int state);
};

