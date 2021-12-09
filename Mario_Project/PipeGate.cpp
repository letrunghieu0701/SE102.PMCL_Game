#include "PipeGate.h"

void CPipeGate::Render()
{
	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);
	float width = right - left;
	float height = bottom - top;


	// To-do: Pipe này có 2 màu, nhưng hiện tại chưa có sprite màu đen, nên chỉ vẽ mỗi màu xanh lá thôi
	
	// Nếu đây là pipe để đi vào Hidden Zone
	// Thì vẽ pipe hướng lên trên
	if (this->gate_in)
		CAnimations::GetInstance()->Get(ID_ANI_PIPE)->Render(x + width / 2, y + height / 2);
	// Còn không thì đây là pipe để đi về mặt đất trong scene 1-1
	// Nên vẽ pipe hướng xuống
	else
		CAnimations::GetInstance()->Get(ID_ANI_PIPE_DOWN)->Render(x + width / 2, y + height / 2);

	RenderBoundingBox();
}
