#pragma once
#include "Pipe.h"

class CPipeGate: public CPipe
{
private:
	int pipe_des_id;
public:
	CPipeGate(float x, float y, int type, int id) : CPipe(x, y, type)
	{
		this->pipe_des_id = id;
	}
	void Render();
	void Update(DWORD dt) {};

	int GetPipeDesID() { return this->pipe_des_id; }
};

