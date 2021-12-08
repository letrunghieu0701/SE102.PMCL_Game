#include "PipeTeleportDestination.h"

void CPipeTeleportDestination::Render()
{
	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);
	float width = right - left;
	float height = bottom - top;


	// To-do: Pipe này có 2 màu, nhưng hiện tại chưa có sprite màu đen, nên chỉ vẽ mỗi màu xanh lá thôi
	CAnimations::GetInstance()->Get(ID_ANI_PIPE_TELE_DES)->Render(x + width / 2, y + height / 2);

	RenderBoundingBox();
}
