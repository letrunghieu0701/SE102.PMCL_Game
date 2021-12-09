#include "PipeTeleportDestination.h"

void CPipeTeleportDestination::Render()
{
	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);
	float width = right - left;
	float height = bottom - top;


	// To-do: Pipe này có 2 màu, nhưng hiện tại chưa có sprite màu đen, nên chỉ vẽ mỗi màu xanh lá thôi

	// Đây là pipe để về mặt đất của scene 1-1, pipe này hướng lên trên
	if (this->gate_out)
		CAnimations::GetInstance()->Get(ID_ANI_PIPE)->Render(x + width / 2, y + height / 2);
	else
	// Đây là pipe thả Mario vào Hidden Zone, pipe này nằm trong Hidden Zone
		CAnimations::GetInstance()->Get(ID_ANI_PIPE_DOWN)->Render(x + width / 2, y + height / 2);

	RenderBoundingBox();
}
