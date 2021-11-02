#include "Tile.h"

void CTile::Render()
{
	CSprites::GetInstance()->Get(this->sprite_id)->Draw(this->x, this->y);
}
