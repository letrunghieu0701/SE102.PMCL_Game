#pragma once
#include "Pipe.h"

#define PIPE_GATE_WAY_IN 1

class CPipeGate: public CPipe
{
private:
	int pipe_des_id;
	bool gate_in;
public:
	CPipeGate(float x, float y, int type, int id, bool is_way_in) : CPipe(x, y, type)
	{
		this->pipe_des_id = id;
		this->gate_in = is_way_in;
	}
	void Render();
	void Update(DWORD dt) {};

	int GetPipeDesID() { return this->pipe_des_id; }
	bool IsGateIn() { return this->gate_in; }
	float GetPosY() { return this->y; }
};

