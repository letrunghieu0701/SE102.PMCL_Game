#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "debug.h"


class CInvisiblePlatform: public CGameObject
{
private:
	int width, height;
	int type;
public:
	CInvisiblePlatform(int x, int y, int type, int width, int height) : CGameObject(x, y, type)
	{ this->width = width;
	this->height = height;
	this->type = type; }

	void Render();
	void Update(DWORD dt){}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

	int IsBlocking() { return 0; }
};

