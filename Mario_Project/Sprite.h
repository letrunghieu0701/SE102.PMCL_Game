#pragma once

#include "Texture.h"
#include "Game.h"
#include "AssetIDs.h"

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

class CSprite
{
private:
	int id;				// Sprite ID in the sprite database

	int left;
	int top;
	int right;
	int bottom;

	LPTEXTURE texture;
	D3DX10_SPRITE sprite;
	D3DXMATRIX matScaling;
public:
	CSprite(int id, int left, int top, int right, int bottom, LPTEXTURE tex);

	int GetLeft() { return this->left; }
	int GetTop() { return this->top; }
	int GetRight() { return this->right; }
	int GetBottom() { return this->bottom; }

	void Draw(float x, float y);
};

typedef CSprite* LPSPRITE;