#pragma once
#include "GameObject.h"
#include "Textures.h"


// Time
#define TAIL_LIFE_TIME 500

class CTail: public CGameObject
{
	float width;
	float height;

	ULONGLONG living_start;
public:
	CTail(float x, float y, float width, float height, ULONGLONG time_start_living, int type) :CGameObject(x, y, type)
	{
		this->width = width;
		this->height = height;
		this->living_start = time_start_living;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	void RenderBoundingBox(void);
	int IsBlocking() { return 0; }

	bool StillHaveLivingTimeLeft();
};

