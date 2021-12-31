#pragma once
#include "PlayScene.h"
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"


#define HUD_BBOX_WIDTH 153
#define HUD_BBOX_HEIGHT 30

#define NUM_SPEED_ARROW 6
#define SPEED_ARROW_POSITION_RELATE_2_HUD_X 52
#define SPEED_ARROW_POSITION_RELATE_2_HUD_Y 10
#define SPEED_ARROW_WIDTH 8
#define SPEED_ARROW_HEIGHT 7

#define P_CHAR_WIDTH 15
#define P_CHAR_HEIGHT 7
#define P_CHAR_DISTANCE_2_ARROWS 5


#define ID_ANI_P_CHAR_NORMAL	16002
#define ID_ANI_P_CHAR_MAX_SPEED	16003

class CHUD : public CGameObject {
public:
	CHUD(float x, float y, int type) : CGameObject(x, y, type) {}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

