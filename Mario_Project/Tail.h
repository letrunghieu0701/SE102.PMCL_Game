#pragma once
#include "GameObject.h"
#include "Textures.h"


class CTail: public CGameObject
{
	float width;
	float height;
public:
	CTail(float x, float y, float width, float height, int type) :CGameObject(x, y, type)
	{
		this->width = width;
		this->height = height;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	void RenderBoundingBox(void);
	int IsBlocking() { return 0; }
};

