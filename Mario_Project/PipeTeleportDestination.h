#pragma once
#include "Pipe.h"

#define PIPE_TELE_DES_SLOW_DOWN_MARIO 1

#define ID_ANI_PIPE_TELE_DES 14010

class CPipeTeleportDestination: public CPipe
{
private:
	int id;
	bool slow_down_mario;
public:
	CPipeTeleportDestination(float x, float y, int type, int id, bool isSlowDown) : CPipe(x, y, type)
	{
		this->id = id;
		this->slow_down_mario = isSlowDown;
	}
	void Render();
	void Update(DWORD dt) {};
};

