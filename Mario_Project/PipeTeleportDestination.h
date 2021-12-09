#pragma once
#include "Pipe.h"

#define PIPE_TELE_DES_BACK_TO_SURFACE 1


class CPipeTeleportDestination: public CPipe
{
private:
	int id;
	bool gate_out;	// Đây là Pipe ở trên mặt đất trong scene 1-1, để Mario trở về mặt đất
public:
	CPipeTeleportDestination(float x, float y, int type, int id, bool is_gate_out) : CPipe(x, y, type)
	{
		this->id = id;
		this->gate_out = is_gate_out;
	}
	void Render();
	void Update(DWORD dt) {};
	bool IsGateOut() { return this->gate_out; }
	float GetPosY() { return this->y; }
};

