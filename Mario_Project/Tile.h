#pragma once
#include "Game.h"
#include "Sprites.h"

#define DEFAULT_TILE_WIDTH 16
#define DEFAULT_TILE_HEIGHT 16

class CTile
{
private:
	int x, y;
	int sprite_id;
public:
	CTile()
	{
		this->x = 0;
		this->y = 0;
		this->sprite_id = -1;
	}
	CTile(int x, int y, int id)
	{
		this->x = x;
		this->y = y;
		this->sprite_id = id;
	}
	int GetX() { return this->x; }
	int GetY() { return this->y; }

	void Render();
};
typedef CTile* LPTILE;

