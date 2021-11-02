#include "ImageMap.h"


void CImageMap::Render()
{
	CSprite* s = CSprites::GetInstance()->Get(this->spriteId);

	s->Draw(x, y);
}


