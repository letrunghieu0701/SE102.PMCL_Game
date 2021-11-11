#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

// Bounding Box
#define PIPE_BBOX_WIDTH 32
#define PIPE_BBOX_HEIGHT 32

// Animation ID
#define ID_ANI_PIPE 14000


class CPipe: public CGameObject
{
public:
	CPipe(float x, float y, int type) : CGameObject(x, y, type) {}
	void Render();
	void Update(DWORD dt);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetState(int state);
};

