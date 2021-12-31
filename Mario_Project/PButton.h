#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"


#define ID_ANI_PBUTTON_BLUE 16000
#define ID_ANI_PBUTTON_PRESSED	16001
#define PBUTTON_BBOX_WIDTH 16
#define PBUTTON_BBOX_HEIGHT 17

#define PBUTTON_PRESSED_BBOX_WIDTH	16
#define PBUTTON_PRESSED_BBOX_HEIGHT	6

#define PBUTTON_PUSH_DOWN_DISTANCE	(PBUTTON_BBOX_HEIGHT - PBUTTON_PRESSED_BBOX_HEIGHT)

#define PBUTTON_STATE_NORMAL	0
#define PBUTTON_STATE_PRESSED	1


class CPButton: public CGameObject
{
public:
	CPButton(float x, float y, int type);
	void virtual Render();
	void virtual Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void virtual GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void virtual SetState(int state);
};

