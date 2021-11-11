#pragma once
#include <string>
#include "Game.h"
#include "Textures.h"
#include "Sprites.h"
#include "GameObject.h"

// Bounding Box
#define IMAGE_MAP_BBOX_WIDTH 2816
#define IMAGE_MAP_BBOX_HEIGHT 626

// Animation ID
#define ID_ANI_IMAGE_MAP 13000

using namespace std;

class CImageMap: public CGameObject
{
private:
	int spriteId;
public:
	CImageMap(float x, float y, int type, int id) :CGameObject(x, y, type) { this->spriteId = id; };
	void Update(DWORD dt) {};
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

