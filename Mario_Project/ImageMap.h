#pragma once
#include <string>
#include "Game.h"
#include "Textures.h"
#include "Sprites.h"
#include "GameObject.h"

using namespace std;

class CImageMap: public CGameObject
{
private:
	int spriteId;
public:
	CImageMap(float x, float y, int type, int id) :CGameObject(x, y, type) { this->spriteId = id; };
	void Update(DWORD dt) {};
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

